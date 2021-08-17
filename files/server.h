#ifndef __SERVER_H__
#define __SERVER_H__

#include <netinet/in.h> 


#define DEFAULT_STRLEN	100
#define CSVDB_RECORDS	25
#define CSVDB_FILENAME  "scifi.csv"
#define DB_SERVER_PORT  12345

/**
 * Movie data structure
 */ 
struct movie 
{
	char title[DEFAULT_STRLEN];
	char director[DEFAULT_STRLEN];
	short year;
	short oscars_won;
	char origin_country[DEFAULT_STRLEN];
};

typedef struct movie movie_t;


#endif /* __SERVER_H__ */
