#include "stdafx.h"
#include "pch.h"
#include "mp3file.h"

Mp3File::Mp3File()
{
    memset( m_title, 0, sizeof m_title );
    memset( m_artist, 0, sizeof m_artist );
    memset( m_album, 0, sizeof m_album );
    memset( m_comments, 0, sizeof m_comments );
	m_year = 0;
    memset( m_nullByte, 0, sizeof m_nullByte );
	m_trackNumber = 0;
	m_genre = 0;
}

Mp3File::~Mp3File()
{
}

const char* Mp3File::GetTitle()const
{
	return m_title;
}

const char* Mp3File::GetArtist()const
{
	return m_artist;
}

const char* Mp3File::GetAlbum() const
{
	return m_album;
}

const char* Mp3File::GetComments() const
{
	return m_comments;
}

int Mp3File::GetYear() const
{
	return m_year;
}

const char Mp3File::GetGenre() const
{
	return m_genre;
}

int Mp3File::GetTrackNumber() const
{
	return m_trackNumber;
}

bool Mp3File::Open(const char* pszFullFilePath)
{
	if (NULL == pszFullFilePath)
		return false;

    FILE *song_file = NULL;
    errno_t err = fopen_s( &song_file, pszFullFilePath, "r+b" );
    if ( 0 != err ) {
        return false;
    }
    fclose( song_file );

	struct ID3V1 id3v1Data;
    struct ID3V2Header id3v2HData;
    ifstream iFile( pszFullFilePath );
    if ( iFile ) {
        iFile.read( reinterpret_cast<char*>(&id3v2HData), sizeof( id3v2HData ) );
        if ( 0 == strcmp( id3v2HData.Header, "ID3" ) ) {
            iFile.close();
        }
        else {
            iFile.seekg( -128, ios::end );
            iFile.read( reinterpret_cast<char*>(&id3v1Data), sizeof( id3v1Data ) );
            iFile.close();

            strncpy_s( m_title, sizeof( m_title ), id3v1Data.title, sizeof( m_title ) - 1 );
            strcat_s( m_title, sizeof(m_title), "\0" );
            strncpy_s( m_artist, sizeof(m_artist), id3v1Data.artist, sizeof( m_artist ) - 1 );
            strcat_s( m_artist, sizeof(m_artist), "\0" );
            strncpy_s( m_album, sizeof(m_album), id3v1Data.album, sizeof( m_album ) - 1 );
            strcat_s( m_album, sizeof(m_album), "\0" );
            strncpy_s( m_comments, sizeof(m_comments), id3v1Data.comments, sizeof( m_comments ) - 1 );
            strcat_s( m_comments, sizeof(m_comments), "\0" );
        }

        return true;
    }

    return false;
}