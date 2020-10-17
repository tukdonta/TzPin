#include <iostream>
#include <vector>
#include <windows.h>
#include <Wincrypt.h>

#include "hash.h"
#include "lib.h"

Hash::Hash() 
{			
	error = false;

	// Получаем дескриптор крипто-провайдера
	if (!CryptAcquireContext(&hCryptProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		hCryptProv = NULL;
		error = true;
	}
}

Hash::~Hash() 
{
	dispose();
}

void Hash::dispose() 
{
	if (hCryptProv)
		CryptReleaseContext(hCryptProv, 0);
}

bool Hash::IsError()
{
	return error;
}

char* Hash::GetMD5(int pin)
{			
	HCRYPTHASH hHash = NULL;
	DWORD cbHashSize = 0;
	DWORD dwCount = sizeof(DWORD);
	char* strHash;

	if (!CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash))
	{
		dispose();
		error = true;
		return NULL;
	}

	memset(buff_pin, '\0', len_buff_pin);
	snprintf(buff_pin, len_buff_pin, "%08d", pin);

	error = !CryptHashData(hHash, (const BYTE*)(buff_pin), len_buff_pin - 1, 0);
	if (!error)
		error = !CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE *)&cbHashSize, &dwCount, 0);
	
	if (!error)
		error = !CryptGetHashParam(hHash, HP_HASHVAL, buff_result, &cbHashSize, 0);
	
	// Если произошла ошибка, чистим ресурсы и выходим
	if (error) {
		CryptDestroyHash(hHash);
		dispose();
		error = false;
		return NULL;
	}

	// Форматируем результат в строковый вид
	strHash = (char*) malloc(BUFF_MD5_STRING);
	memset(strHash, '\0', BUFF_MD5_STRING);
	for (int i = 0; i < cbHashSize; i++)
	{
		strHash[i * 2] = hex[buff_result[i] >> 4];
		strHash[(i * 2) + 1] = hex[buff_result[i] & 0xF];
	}
	
	// Закрываем дескриптор	
	CryptDestroyHash(hHash);	

	return strHash;
}
