#ifndef LList_h
#define LList_h

#include <Arduino.h>
#include <stdint.h>
#include "ProjectConfig.h"
#include "Buffer.h"


typedef struct note{
	uint8_t index;
	char* value1;
	char* value2;
	struct note* next;
}note_t;


class LList {

public:

	// Constructor of class LList
	LList();

	// add string into structures
	uint8_t add(uint8_t index, char* value1);

	// add string into structuress for two varible
	uint8_t add(uint8_t index, char* value1, char* value2);

	// add malloced string into structuress for two varible
	uint8_t add_malloced(uint8_t index, char* value1, char* value2);

	// get value1 from structure by index
	char* get(uint8_t index);

	// get addres into note from structure by index
	note_t* get_note(uint8_t index);

	// get value1 from note
	char* get_value1(note_t* one);

	// get value1 from note
	char* get_value2(note_t* one);

	// remove note from structure by index
	void remove(uint8_t index);

	// remove note from structure by value1
	void removeByValue1(char* value1);

	// remove all menbers from strusture
	void flush();

	// return size of structure
	uint8_t getSize();

	// return max number of index from structure
	uint8_t getMaxIndex();

	// Print value1 from structure into Buffre
	char* Bprint();

	// Print structure wiht pointers from structure into Buffre
	char* Bprint_list();


private:

	note_t* _first_note;
	note_t* _last_note;
	uint8_t _size;

	// add note into structure
	void addNote(note_t* one);

	// Conpare string (max lenght uint16_t)
	uint8_t StrCompare(char* str1, char* str2);

};


#endif
