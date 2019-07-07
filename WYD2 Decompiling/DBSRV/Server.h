#ifndef SERVER_H
#define SERVER_H

void ReadTrandGuildInfo();
void WriteTrandGuildInfo();
void ConvertGuildNumber(int32_t srv, STRUCT_ITEM* item);
bool SendAdminSignal(int32_t svr, uint16_t id, uint16_t signal);
bool SendAdminMessage(int32_t svr, uint16_t id, char* message);
bool SendAdminParm(int32_t svr, uint16_t id, uint16_t signal, int32_t parm);
bool SendAdminState(int32_t svr, uint16_t id, uint16_t signal, char* pass);
bool InitApplication(HINSTANCE hInstance);
bool InitInstance(HINSTANCE hInstance, int32_t nCmdShow);
void UpdateConnection();
void UpdateConnectionData();
void ReadAdmin();
bool ReadConfig();
bool WriteConfig();
bool ProcessAdminClientMessage(uint8_t* msg);
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ProcessClientMessage(int32_t conn, uint8_t* msg);
void DisableAccount(int32_t conn, char* account);
void EnableAccount(int32_t conn, char * account);
bool ProcessAdminMessage(int32_t conn, uint8_t* msg);
void ProcessSecTimer();
void ProcessMinTimer();
void StartLog(const char* str);
void Log(const char* str, ...);
int32_t GetUserFromSocket(uint32_t sock);
int32_t GetAdminFromSocket(uint32_t sock);
int32_t GetEmptyUser();
void UpdateUser();
void ImportItem();
void ImportUser();

#endif // !SERVER_H
