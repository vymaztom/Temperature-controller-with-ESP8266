#include <FS.h>
#include <LittleFS.h>


#ifndef WIFICONFIGJSON
#define WIFICONFIGJSON


class WifiConfigJson {

	private:
		void ReadConfig();

		void readFile(const char * path);

		void writeFile(const char * path, const char * message);

		void renameFile(const char * path1, const char * path2);

		void deleteFile(const char * path);

	public:
		WifiConfigJson();
		void listDir(const char * dirname);

};




#endif
