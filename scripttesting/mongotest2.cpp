#include <cstdlib>
#include <iostream>
#include "mongo/client/dbclient.h" // for the driver

void run() {
  mongo::DBClientConnection c;
  c.connect("localhost");
}

int main() {

  mongo::BSONObj credentials = BSON(
  "user" << "sichaud" <<
  "userSource" << "etbackend" <<
  "pwd" << "123test" <<
  "mechanism" << "MONGODB-CR" <<
  "serviceHostname" << "ds049651.mongolab.com:49651"
);


  mongo::client::initialize();
  mongo::DBClientConnection c;
  c.connect("ds049651.mongolab.com:49651");

    c.auth(credentials);





    try {

      std::auto_ptr<mongo::DBClientCursor> cursor =
        c.query("etbackend.appointments", mongo::Query(""));

     int counter = 0;
        while(cursor->more()){
          mongo::BSONObj rowRes = cursor->nextSafe();

	std::set <std::string> fields;
	rowRes.getFieldNames(fields);
	std::set<std::string>::iterator setIterator;


          counter++;
        }

        std::cout << "connected ok" << "count is: " << counter << std::endl;
    } catch( const mongo::DBException &e ) {
        std::cout << "caught " << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
