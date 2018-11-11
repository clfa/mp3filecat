#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <assert.h>

using namespace std;

class Mp3File
{
public:
	Mp3File();
	virtual ~Mp3File();


	bool Open(const char* pszFullFilePath);

	const char* GetTitle() const;
	const char* GetArtist() const;
	const char* GetAlbum() const;
	const char* GetComments() const;
	const char GetGenre() const;
	int GetYear() const;
	int GetTrackNumber() const;

private:
	char m_title[31];
	char m_artist[31];
	char m_album[31];
	char m_comments[29];
	char m_year;
	char m_nullByte[1];
	unsigned char m_trackNumber;
	unsigned char m_genre;
};

struct ID3V1
{
    char tag[3];                // 标签头，必须为"TAG"
	char title[30];             // 标题
	char artist[30];            // 作者
	char album[30];             // 专辑
	char year[4];               // 出品年代
	char comments[28];          // 备注
	char nullByte;              // 保留
	unsigned char trackNumber;  // 音轨
	unsigned char genre;        // 类型
};

struct ID3V2Header {
    char Header[3];             // 标签，必须为ID3
    char Ver;                   // 版本号
    char Reversion;             // 副版本号
    char Flag;                  // 存放标志的字节
    char Size[4];               // 标签大小
};