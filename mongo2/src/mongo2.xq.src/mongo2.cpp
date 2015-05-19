#include <algorithm>
#include <cstdio>
#include <time.h>
#include <iostream>
#include <sstream>

#include <zorba/empty_sequence.h>
#include <zorba/item_factory.h>
#include <zorba/serializer.h>
#include <zorba/singleton_item_sequence.h>
#include <zorba/store_manager.h>
#include <zorba/user_exception.h>
#include <zorba/util/transcode_stream.h>
#include <zorba/util/uuid.h>
#include <zorba/vector_item_sequence.h>

#include <cstdlib>

#include "mongo2.h"

namespace zorba {
	namespace mongo2 {

		zorba::ExternalFunction*
			Mongo2Module::getExternalFunction(const zorba::String& localname)
		{

			if (localname == "query"){
				return new QueryFunction(this);
			}
			else if (localname == "count"){
				return new CountFunction(this);
			}

			return 0;
		}

		void Mongo2Module::destroy()
		{
			if (!dynamic_cast<Mongo2Module*>(this))
			{
				return;
			}
			delete this;
		}

		Mongo2Module::~Mongo2Module()
		{
			for (FuncMap_t::const_iterator lIter = theFunctions.begin();
				lIter != theFunctions.end();
				++lIter)
			{
				delete lIter->second;
			}
			theFunctions.clear();
		}

		/*********************************************************/

		InstanceMap::InstanceMap()
		{
			InstanceMap::instanceMap = new InstanceMap_t();
		}

		bool
			InstanceMap::storeInstance(String& aKeyName, mdb_conn* aInstance)
		{
			std::pair<InstanceMap_t::iterator, bool> ret;
			ret = instanceMap->insert(std::pair<String, mdb_conn*>(aKeyName, aInstance));
			return ret.second;
		}

		mdb_conn*
			InstanceMap::getInstance(const String& aKeyName)
		{
			InstanceMap::InstanceMap_t::iterator lIter = instanceMap->find(aKeyName);

			if (lIter == instanceMap->end())
				return NULL;

			mdb_conn* lInstance = lIter->second;

			return lInstance;
		}

		bool
			InstanceMap::deleteInstance(String& aKeyName)
		{
			InstanceMap::InstanceMap_t::iterator lIter = instanceMap->find(aKeyName);

			if (lIter == instanceMap->end())
				return false;

			//lcb_destroy(lIter->second);

			instanceMap->erase(lIter);

			return true;
		}

		/**********************************************************/

		//The following 'Mongo2ItemSequence' caused aborts after ca. 102 objects have been read.
		//Please use the Mongo2ItemSequence5 as it's actually used in the query and count function


		//void Mongo2ItemSequence::Mongo2Iterator::open()
		//{
		//	//if(_resultCursor == NULL)
		//	//  return;

		//	itemFactory = Zorba::getInstance(0)->getItemFactory();
		//	_isOpenVal = true;
		//	fcount = 0;
		//	resultPtr = _resultCursor.get();
		//	_resultCursor.release();
		//}

		//bool Mongo2ItemSequence::Mongo2Iterator::getItemFromBSONObj(mongo::BSONObj bsonObj, zorba::Item &aItem)
		//{
		//	std::cout << "step_getItemFromBSONObj" << std::endl;
		//	std::vector<std::pair<zorba::Item, zorba::Item> > pairs;
		//	/*std::set< std::string > fields;
		//	bsonObj.getFieldNames(fields);
		//	std::set<std::string>::iterator setIterator;
		//	for (setIterator = fields.begin(); setIterator != fields.end(); ++setIterator)
		//	{
		//	mongo::BSONElement elemValue = bsonObj.getField(*setIterator);
		//	Item valItem;
		//	if(getItemFromBSONElement(elemValue, valItem))
		//	{
		//	pairs.push_back(std::pair<Item, Item>(
		//	itemFactory->createString(*setIterator),
		//	valItem)
		//	);
		//	}
		//	}
		//	*/
		//	aItem = itemFactory->createJSONObject(pairs);
		//	pairs.clear();
		//	//aItem = itemFactory->createString("connection fail") ;
		//	return true;
		//}

		//bool Mongo2ItemSequence::Mongo2Iterator::getItemFromBSONElement(mongo::BSONElement elemValue, zorba::Item &valItem)
		//{

		//	//definitions for case array have to be before the switch statement
		//	std::vector<mongo::BSONElement> arr;
		//	std::vector<Item> arrItems;

		//	switch (elemValue.type())
		//	{
		//	case mongo::NumberDouble:
		//		valItem = itemFactory->createDouble(elemValue.numberDouble());
		//		break;
		//	case mongo::NumberLong:
		//		valItem = itemFactory->createLong(elemValue.Long());
		//		break;
		//	case mongo::NumberInt:
		//		valItem = itemFactory->createInt(elemValue.Int());
		//		break;
		//	case mongo::String:
		//		valItem = itemFactory->createString(elemValue.String());
		//		break;
		//	case mongo::Bool:
		//		valItem = itemFactory->createBoolean(elemValue.Bool());
		//		break;
		//	case mongo::jstNULL:
		//		valItem = itemFactory->createJSONNull();
		//		break;

		//		/* TODO
		//		case mongo::BSONType.Date :
		//		valItem = itemFactory->createDateTimeStamp(elemValue.numberDouble());
		//		break;
		//		case mongo::BSONType.Timestamp  :
		//		valItem = itemFactory->createTime(elemValue.numberDouble());
		//		break;


		//		case mongo::Array :
		//		arr = elemValue.Array();
		//		for(std::vector<mongo::BSONElement>::iterator it = arr.begin(); it != arr.end(); ++it)
		//		{
		//		Item arrElemItem;
		//		if(getItemFromBSONElement(*it, arrElemItem))
		//		arrItems.push_back(arrElemItem);
		//		}
		//		valItem = itemFactory->createJSONArray(arrItems);
		//		break;

		//		case mongo::Object :
		//		getItemFromBSONObj(elemValue.Obj(), valItem);
		//		break;
		//		*/

		//	default: return false;
		//	}

		//	return true;
		//}

		//bool Mongo2ItemSequence::Mongo2Iterator::next(zorba::Item &aItem)
		//{
		//	fcount++;
		//	std::cout << "fcount: " << fcount << std::endl;
		//	std::cout << "step1" << std::endl;
		//	//std::cout << "pointer points to : " << resultPtr << std::endl;

		//	if (!(resultPtr->more()))
		//	{
		//		return false;
		//	}

		//	try{

		//		bool test = false;
		//		if (!test)
		//		{

		//			std::cout << "NOTEST_step2" << std::endl;
		//			mongo::BSONObj rowRes = resultPtr->next();
		//			std::cout << "step3" << std::endl;
		//			if (getItemFromBSONObj(rowRes, aItem))
		//			{

		//				return true;
		//			}
		//			else{
		//				aItem = itemFactory->createJSONNull();
		//				return true;
		//			}

		//		}
		//		else{


		//			std::cout << "step2" << std::endl;
		//			mongo::BSONObj rowRes = resultPtr->nextSafe();
		//			std::cout << "step3" << std::endl;
		//			aItem = itemFactory->createJSONNull();
		//			//aItem = itemFactory->createString("c1") ;
		//			/*  if( getItemFromBSONObj(rowRes, aItem))
		//			{


		//			//  std::cout << "aItem String value is: " << aItem.getStringValue().c_str() << std::endl;
		//			std::cout << "step44" << std::endl;
		//			return true;
		//			}else{
		//			aItem = itemFactory->createJSONNull();
		//			return true;
		//			}
		//			*/
		//			std::cout << "step9" << std::endl;
		//			return true;

		//		}


		//	}
		//	catch (...){
		//		std::cout << "landed in catch" << std::endl;
		//		return false;
		//	}

		//	std::cout << "step 99 ending" << std::endl;
		//	return true;
		//}

		//void Mongo2ItemSequence::Mongo2Iterator::close()
		//{
		//	_isOpenVal = false;
		//}


		/**********************************************************/

		/**********************************************************/

		void Mongo2ItemSequence5::Mongo2Iterator5::open()
		{
			_isOpenVal = true;
			fcount = 0;
			_it = _itms->begin();
		}

		bool Mongo2ItemSequence5::Mongo2Iterator5::getItemFromBSONObj(mongo::BSONObj bsonObj, zorba::Item &aItem)
		{
			return true;
		}

		bool Mongo2ItemSequence5::Mongo2Iterator5::getItemFromBSONElement(mongo::BSONElement elemValue, zorba::Item &valItem)
		{
			return true;
		}

		bool Mongo2ItemSequence5::Mongo2Iterator5::next(zorba::Item &aItem)
		{
			fcount++;

			try{

				if (_it != _itms->end())
				{
					aItem = *_it;
					_it++;
					return true;
				}
				else{
					return false;
				}

			}
			catch (...){
				std::cout << "landed in catch of: Mongo2ItemSequence5::Mongo2Iterator5::next" << std::endl;
				return false;
			}

			return true;
		}

		void Mongo2ItemSequence5::Mongo2Iterator5::close()
		{
			_isOpenVal = false;
		}


		/**********************************************************/

		MongoBaseFunction::MongoBaseFunction(const Mongo2Module* aModule)
			: theModule(aModule)
		{
		}

		MongoBaseFunction::~MongoBaseFunction()
		{
		}

		String
			MongoBaseFunction::getURI() const
		{
			return theModule->getURI();
		}

		String
			MongoBaseFunction::getOneStringArgument(const Arguments_t& aArgs, int aPos) const
		{
			Item lItem = getOneItemArgument(aArgs, aPos);
			String lTmpString = lItem.getStringValue();
			return lTmpString;
		}

		Item
			MongoBaseFunction::getOneItemArgument(const Arguments_t& aArgs, int aPos) const
		{
			Item lItem;
			Iterator_t args_iter = aArgs[aPos]->getIterator();
			args_iter->open();
			args_iter->next(lItem);
			args_iter->close();
			return lItem;
		}

		void
			MongoBaseFunction::throwError(const char *aLocalName, const char* aErrorMessage) const
		{
			String errName(aLocalName);
			Item errQName = theModule->getItemFactory()->createQName(MONGO2_MODULE_NAMESPACE, errName);
			String errDescription(aErrorMessage);
			throw USER_EXCEPTION(errQName, errDescription);
		}

		void
			MongoBaseFunction::isNotJSONError() const
		{
			throwError("CB0002", "Options parameter is not a JSON object");
		}

		mdb_conn*
			MongoBaseFunction::getInstance(const DynamicContext* aDctx, const String& aIdent) const
		{
			InstanceMap* lInstanceMap;
			mdb_conn* lInstance;
			if ((lInstanceMap = dynamic_cast<InstanceMap*>(aDctx->getExternalFunctionParameter("mongoInstanceMap"))))
			{
				lInstance = lInstanceMap->getInstance(aIdent);
				return lInstance;
			}
			throwError("CB0000", "No instance of mongo with the given identifier was found.");
			return NULL;
		}

		/**********************************************************/

		std::string
      		getTimestampStringFromSeconds(uint32_t sec)
		{
			time_t time = sec;
			struct tm * timeinfo;
			timeinfo = gmtime(&time);
			char buffer [80];
			strftime(buffer, 80, "%Y-%m-%dT%H:%M:%S-00:00",timeinfo);
			return std::string(buffer);
		}
		
		std::string getDateStringFromTimet(time_t tt)
		{
			struct tm * timeinfo;
			timeinfo = gmtime(&tt);
			char buffer [80];
			strftime(buffer, 80, "%Y-%m-%d",timeinfo);
			return std::string(buffer);			
		}



		bool getItemFromBSONElement(ItemFactory *itemFactory, mongo::BSONElement elemValue, zorba::Item &valItem)
		{

			//definitions for case array have to be before the switch statement
			std::vector<mongo::BSONElement> arr;
			std::vector<Item> arrItems;
			std::string gdate;

			switch (elemValue.type())
			{
				case mongo::NumberDouble:
					valItem = itemFactory->createDouble(elemValue.numberDouble());
					break;
				case mongo::NumberLong:
					valItem = itemFactory->createLong(elemValue.Long());
					break;
				case mongo::NumberInt:
					valItem = itemFactory->createInt(elemValue.Int());
					break;
				case mongo::String:
					valItem = itemFactory->createString(elemValue.String());
					break;
				case mongo::Bool:
					valItem = itemFactory->createBoolean(elemValue.Bool());
					break;
				case mongo::jstNULL:
					valItem = itemFactory->createJSONNull();
					break;
		/*			
				case mongo::Date :
					valItem = itemFactory->createDateTimeStamp( getTimestampStringFromSeconds((uint32_t) elemValue.Date().asInt64() ));
					break;


			//	case mongo::Date:
			//		valItem = itemFactory->createDateTimeStamp( getTimestampStringFromSeconds( 3286342 ));
			//		break;

				case mongo::Timestamp:
					valItem = itemFactory->createDateTimeStamp(getTimestampStringFromSeconds(elemValue.Timestamp().seconds()));
					break;
		*/			
					
					

				case mongo::Array:
					arr = elemValue.Array();
					for (std::vector<mongo::BSONElement>::iterator it = arr.begin(); it != arr.end(); ++it)
					{
						Item arrElemItem;
						if (getItemFromBSONElement(itemFactory, *it, arrElemItem))
							arrItems.push_back(arrElemItem);
					}
					valItem = itemFactory->createJSONArray(arrItems);
					break;

				case mongo::Object:
					getItemFromBSONObj(itemFactory, elemValue.Obj(), valItem);
					break;


				default:
					return false;
			}

			return true;
		}

		//returns a zorba Item from a Mongo BSONObject. Function can be used recursively
		bool getItemFromBSONObj(ItemFactory *itemFactory, mongo::BSONObj bsonObj, zorba::Item &aItem)
		{
			std::vector<std::pair<zorba::Item, zorba::Item> > pairs;
			std::set< std::string > fields;
			bsonObj.getFieldNames(fields);
			std::set<std::string>::iterator setIterator;
			for (setIterator = fields.begin(); setIterator != fields.end(); ++setIterator)
			{
				mongo::BSONElement elemValue = bsonObj.getField(*setIterator);
				Item valItem;
				if (getItemFromBSONElement(itemFactory, elemValue, valItem))
				{
					pairs.push_back(std::pair<Item, Item>(
						itemFactory->createString(*setIterator),
						valItem)
						);
				}
			}

			aItem = itemFactory->createJSONObject(pairs);
			pairs.clear();
			return true;
		}





		zorba::ItemSequence_t
			QueryFunction::evaluate(
			const ExternalFunction::Arguments_t& args,
			const StaticContext* sctx,
			const DynamicContext* dctx) const{

			try {

				Item credItem = getOneItemArgument(args, 0);
				std::string collectionStr = getOneStringArgument(args, 1).c_str();
				std::string queryStr = getOneStringArgument(args, 2).c_str();


				//retrieve credentials from query-command params
				mongo::BSONObjBuilder b;
				std::string connHost;

				//adds auth mechansim to credentials if set to true and performs authentication
				bool doAuth = false;

				if (!credItem.isJSONItem())
				{
					throwError("mongo_connect_function", "no json found");
				}
				Iterator_t lKeys = credItem.getObjectKeys();
				lKeys->open();
				Item lKey;
				while (lKeys->next(lKey))
				{
					String lStrKey = lKey.getStringValue();
					if (lStrKey == "host" || lStrKey == "serviceHostname")
					{
						connHost = credItem.getObjectValue(lStrKey).getStringValue().c_str();
						b.append("serviceHostname", connHost);
					}
					else if (lStrKey == "user")
					{
						b.append("user", credItem.getObjectValue(lStrKey).getStringValue().c_str());
					}
					else if (lStrKey == "userSource")
					{
						b.append("userSource", credItem.getObjectValue(lStrKey).getStringValue().c_str());
					}
					else if (lStrKey == "pwd")
					{
						b.append("pwd", credItem.getObjectValue(lStrKey).getStringValue().c_str());
						doAuth = true;
					}
				}

				if (doAuth)
				{
					b.append("mechanism", "MONGODB-CR");
				}

				mongo::BSONObj credentials = b.obj();


				/*  //create connection obj
				mongo::BSONObj credentials = BSON(
				"user" << "user1" <<
				"userSource" << "first" <<
				"pwd" << "password1" <<
				"mechanism" << "MONGODB-CR" <<
				"serviceHostname" << "localhost:27017"
				); */

				mongo::client::initialize();
				mongo::DBClientConnection c;
				c.connect(connHost);
				if (doAuth)
				{
					c.auth(credentials);
				}


				std::auto_ptr<mongo::DBClientCursor> c1 =
					c.query(collectionStr, mongo::Query(queryStr));

				std::vector<zorba::Item> * itms = new std::vector<zorba::Item>();
				int counter = 0;
				while (c1->more()){
					zorba::Item aItem;
					if (getItemFromBSONObj(theModule->getItemFactory(), c1->next(), aItem))
					{
						itms->push_back(aItem);
						counter++;
					}
				}







				return ItemSequence_t(new Mongo2ItemSequence5(itms));


				//ALTERNATIVE WAYS ACCORDING TO ZORBA ARCHITECTURE -> Gives errors after ca. 102 elements processed..
				/*
				std::auto_ptr<mongo::DBClientCursor> cursor =
				c.query(collectionStr, mongo::Query(queryStr));
				return ItemSequence_t(new Mongo2ItemSequence(cursor));
				*/

			}
			catch (const mongo::DBException &e) {
				Item item = theModule->getItemFactory()->createString("connection fail");
				throwError("mongo_query_function", e.what());
			}
		}


		zorba::ItemSequence_t
			CountFunction::evaluate(
			const ExternalFunction::Arguments_t& args,
			const StaticContext* sctx,
			const DynamicContext* dctx) const{

			try {

				std::string collectionStr = getOneStringArgument(args, 1).c_str();
				std::string queryStr = getOneStringArgument(args, 2).c_str();

				Item credItem = getOneItemArgument(args, 0);
				mongo::BSONObjBuilder b;
				std::string connHost;
				bool doAuth = false;

				if (!credItem.isJSONItem())
				{
					throwError("mongo_connect_function", "no json found");
				}
				Iterator_t lKeys = credItem.getObjectKeys();
				lKeys->open();
				Item lKey;
				while (lKeys->next(lKey))
				{
					String lStrKey = lKey.getStringValue();
					if (lStrKey == "host" || lStrKey == "serviceHostname")
					{
						connHost = credItem.getObjectValue(lStrKey).getStringValue().c_str();
						b.append("serviceHostname", connHost);
					}
					else if (lStrKey == "user")
					{
						b.append("user", credItem.getObjectValue(lStrKey).getStringValue().c_str());
					}
					else if (lStrKey == "userSource")
					{
						b.append("userSource", credItem.getObjectValue(lStrKey).getStringValue().c_str());
					}
					else if (lStrKey == "pwd")
					{
						b.append("pwd", credItem.getObjectValue(lStrKey).getStringValue().c_str());
						doAuth = true;
					}
				}

				if (doAuth)
				{
					b.append("mechanism", "MONGODB-CR");
				}

				mongo::BSONObj credentials = b.obj();

				mongo::client::initialize();
				mongo::DBClientConnection c;
				c.connect(connHost);
				if (doAuth)
				{
					c.auth(credentials);
				}

				long countResult =
					c.count(collectionStr, mongo::Query(queryStr));

				Item item = theModule->getItemFactory()->createLong(countResult);
				return ItemSequence_t(new SingletonItemSequence(item));

			}
			catch (const mongo::DBException &e) {
				Item item = theModule->getItemFactory()->createString("connection fail");
				throwError("mongo_query_function", e.what());
			}
		}




	}

}



#ifdef WIN32
#  define DLL_EXPORT __declspec(dllexport)
#else
#  define DLL_EXPORT __attribute__ ((visibility("default")))
#endif

extern "C" DLL_EXPORT zorba::ExternalModule* createModule() {
	return new zorba::mongo2::Mongo2Module();
}
