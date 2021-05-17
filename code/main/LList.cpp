#include "LList.h"



// Constructor of class LList
LList::LList(){
	_size = 0;
	_last_note = NULL;
	_first_note = NULL;
}

// add note into structure
void LList::addNote(note_t* one){
	if(_size == 0){
		_first_note = one;
		_last_note = one;
		_size++;
	}else{
		_last_note->next = one;
		_last_note = one;
		_size++;
	}
}

// add string into structuress for one varible
uint8_t LList::add(uint8_t index, char* value1){
	return add(index, value1, " ");
}

// add string into structuress for two varible
uint8_t LList::add(uint8_t index, char* value1, char* value2){

	if(strlen(value1) > MAX_STRING_LENG){
		value1[MAX_STRING_LENG] = '\0';
		#if DEBUG
		Serial.printf("ERR: string to long value1 (reduce to %u chars)\n", strlen(value1));
		#endif
	}
	if(strlen(value2) > MAX_STRING_LENG){
		value2[MAX_STRING_LENG] = '\0';
		#if DEBUG
		Serial.printf("ERR: string to long value12 (reduce to %u chars)\n", strlen(value2));
		#endif
	}

	char* str1 = (char*)malloc(sizeof(char)*(strlen(value1)+1));

	if(str1 == NULL){
		Serial.println("ERROR: value1");
		return 0;
	}

	char* str2 = (char*)malloc(sizeof(char)*(strlen(value2)+1));

	if(str2 == NULL){
		free(str1);
		Serial.println("ERROR: value2");
		return 0;
	}

	strncpy(str1, value1, strlen(value1)+1);
	strncpy(str2, value2, strlen(value2)+1);

	return add_malloced(index, str1, str2);
}

// add malloced string into structuress for two varible
uint8_t LList::add_malloced(uint8_t index, char* value1, char* value2){

	note_t* one = (note_t*)malloc(sizeof(note_t));

	if(one == NULL){
		Serial.println("ERROR: MALLOC STRUCTURE");
		return 0;
	}

	if(strlen(value1) > MAX_STRING_LENG){
		value1[MAX_STRING_LENG] = '\0';
		#if DEBUG
		Serial.printf("ERR: string to long value1 (reduce to %u chars)\n", strlen(value1));
		#endif
	}
	if(strlen(value2) > MAX_STRING_LENG){
		value2[MAX_STRING_LENG] = '\0';
		#if DEBUG
		Serial.printf("ERR: string to long value12 (reduce to %u chars)\n", strlen(value2));
		#endif
	}
	one->value1 = value1;
	one->value2 = value2;
	one->index = index;
	one->next = NULL;

	addNote(one);
	return 1;
}

// remove note from structure by index
void LList::remove(uint8_t index){
	if(_size > 0){
		note_t* last = NULL;
		note_t* one = _first_note;
		while(one != NULL){
			if(one->index == index){
				if(last == NULL){
					_first_note = one->next;
					if(DEBUG){
						Serial.printf("First Remove: %i _first_note add %p free %p size: %u \n",one->index, _first_note, one, _size);
					}
					free(one->value1);
					free(one->value2);
					free(one);
				}else{
					last->next = one->next;
					if(DEBUG){
						Serial.printf("Other Remove: %i _first_note add %p free %p size: %u \n",one->index, _first_note, one, _size);
					}
					if(one->next == NULL){
						_last_note = last;
					}
					free(one->value1);
					free(one->value2);
					free(one);
				}
				_size--;
			}
			last = one;
			one = one->next;
		}
	}
}

// remove note from structure by value1
void LList::removeByValue1(char* value1){
	if(_size > 0){
		note_t* last = NULL;
		note_t* one = _first_note;
		while(one != NULL){
			if(StrCompare(one->value1, value1)){
				if(last == NULL){
					_first_note = one->next;
					if(DEBUG){
						Serial.printf("First Remove: %i _first_note add %p free %p size: %u \n",one->index, _first_note, one, _size);
					}
					free(one->value1);
					free(one->value2);
					free(one);
				}else{
					last->next = one->next;
					if(one->next == NULL){
						_last_note = last;
					}
					if(DEBUG){
						Serial.printf("Other Remove: %i _first_note add %p free %p size: %u \n",one->index, _first_note, one, _size);
					}
					free(one->value1);
					free(one->value2);
					free(one);
				}
				_size--;
			}
			last = one;
			one = one->next;
		}
	}
}

// remove all menbers from strusture
void LList::flush(){
	while(_size != 0){
		remove(_first_note->index);
	}
	_size = 0;
	_last_note = NULL;
	_first_note = NULL;
}

// return size of structure
uint8_t LList::getSize(){
	return _size;
}

// return max number of index from structure
uint8_t LList::getMaxIndex(){
	uint8_t max = 0;
	if(_size > 0){
		note_t* one = _first_note;
		while(one != NULL){
			if(one->index > max){
				max = one->index;
			}
			one = one->next;
		}
	}
	return max;
}

// Print value1 from structure into Buffre
char* LList::Bprint(){
	Buffer buffer;
	if(_size > 0){
		note_t* one = _first_note;
		buffer.printf("[");
		while(one != NULL){
			buffer.printf("{%i:%s}",one->index, one->value1);
			one = one->next;
		}
		buffer.printf("]\n");
	}
	return buffer.get();
}


// Print structure wiht pointers from structure into Buffre
char* LList::Bprint_list(){
	Buffer buffer;
	buffer.printf("[ size: ");
	if(_size > 0){
		note_t* one = _first_note;
		buffer.printf("%u\n", _size);
		while(one != NULL){
			buffer.printf("{index:%i ; value1:%s ; value2:%s  }\n", one->index, one->value1, one->value2);
			one = one->next;
		}
	}else{
		buffer.printf("0\n{}\n");
	}
	buffer.printf("] \n");
	return buffer.get();
}


// get value1 from structure by index
char* LList::get(uint8_t index){
	if(_size > 0){
		note_t* one = _first_note;
		while(one != NULL){
			if(one->index == index){
				return one->value1;
			}
			one = one->next;
		}
	}
	return NULL;
}


// get addres into note from structure by index
note_t* LList::get_note(uint8_t index){
	if(_size > 0){
		note_t* one = _first_note;
		while(one != NULL){
			if(one->index == index){
				return one;
			}
			one = one->next;
		}
	}
	return NULL;
}


// get value1 from note
char* LList::get_value1(note_t* one){
	return one->value1;
}

// get value1 from note
char* LList::get_value2(note_t* one){
	return one->value2;
}

// Conpare string (max lenght uint16_t)
uint8_t LList::StrCompare(char* str1, char* str2){
	uint16_t len1 = strlen(str1);
	uint16_t len2 = strlen(str2);
	if(len1 == len2){
		for(uint16_t i = 0 ; i < len1 ; i++){
			if(str1[i] != str2[i]){
				return 0;
			}
		}
		return 1;
	}
	return 0;
}
