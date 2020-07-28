#ifndef PREDEFINED_H
#define PREDEFINED_H

#include <string.h>

namespace bpt{
	// predefined B+ info
	#define BP_ORDER 50

// key/value type
	struct value_t{
		char name[256], email[256];
		int age;
	};

	struct key_t{
		char key[16];
		
		key_t(const char *str= ""){
			bzero(key, sizeof(key));
			strcpy(key, str);
		}
	};

	inline int keycmp(const key_t &first, const key_t &second){
		int diff = strlen(first.key) - strlen(second.key);
		return diff == 0 ? strcmp(first.key, second.key): diff;
	}

	#define OPERATOR_KEYCMP(type) \
    bool operator< (const key_t &l, const type &r) {\
        return keycmp(l, r.key) < 0;\
    }\
    bool operator< (const type &l, const key_t &r) {\
        return keycmp(l.key, r) < 0;\
    }\
    bool operator== (const key_t &l, const type &r) {\
        return keycmp(l, r.key) == 0;\
    }\
    bool operator== (const type &l, const key_t &r) {\
        return keycmp(l.key, r) == 0;\
    }

}

#endif 
