#include <stdio.h>
#include <Windows.h>
#include <sddl.h> // Windows API security and identity stuff

const char* hosts_file_path = "C:\\Windows\\System32\\drivers\\etc\\hosts";
const char* localhost = "127.0.0.1";

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("\nType the hostnames of websites to block as arguments\nRun wblock -help for details\n\n");
        return 1;
    }

    if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-?") == 0 || strcmp(argv[1], "-help") == 0)
    {
        printf("\n| Website Blocker\n");
        printf("Usage: wblock hostname\n");
        printf("Example: wblock somewebsite.com\n");
        printf("You can specify multiple hostnames: wblock website1.com website2.com website3.com\n");
        printf("You can also block only a specific hostname rather than the entire thing. For instance, running wblock images.google.com won't block everything from google but just google images.\n");
        printf("This program works by editing the hosts file to make blocked hostnames resolve to localhost.\n");

        return 0;
    }

    HANDLE token;
    BOOL is_elevated = FALSE;

    if(!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token))
    {
        printf("Failed to open process token: [%lu]\n", GetLastError());
        return 1;
    }

    SID_IDENTIFIER_AUTHORITY nt_authority = SECURITY_NT_AUTHORITY;
    PSID admin_sid;

    if(!AllocateAndInitializeSid(&nt_authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &admin_sid))
    {
        printf("Failed to create Administrators SID: [%lu]\n", GetLastError());
        CloseHandle(token);
        return 1;
    }

    if(!CheckTokenMembership(NULL, admin_sid, &is_elevated))
    {
        printf("Failed to check token membership: [%lu]\n", GetLastError());
        CloseHandle(token);
        FreeSid(admin_sid);
        return 1;
    }

    CloseHandle(token);
    FreeSid(admin_sid);

    if(is_elevated == FALSE)
    {
        printf("Administrator permissions are required to edit hosts file\n");
        return 0;
    }

    FILE* hosts_file = fopen(hosts_file_path, "a");
    if(hosts_file == NULL)
    {
        printf("Failed to open hosts file\n");
        return 1;
    }

    for(int i = 1; i < argc; i++)
        fprintf(hosts_file, "127.0.0.1 %s\n", argv[i]);

    fclose(hosts_file);

    printf("Done\n");
    return 0;
}
