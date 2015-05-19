#ifndef _COM_ZORBA_WWW_MODULES_MONGO2_H_
#define _COM_ZORBA_WWW_MODULES_MONGO2_H_

#include <map>

#include <zorba/zorba.h>
#include <zorba/external_module.h>
#include <zorba/function.h>
#include <zorba/dynamic_context.h>

#include "/usr/local/include/mongo/client/dbclient.h" // for the driver


#define MONGO2_MODULE_NAMESPACE "http://www.zorba-xquery.com/modules/mongo2"

namespace zorba { namespace mongo2 {

  class Mongo2Module : public ExternalModule {
    private:
      static ItemFactory* theFactory;

    protected:
      class ltstr
      {
      public:
        bool operator()(const String& s1, const String& s2) const
        {
          return s1.compare(s2) < 0;
        }
      };

      typedef std::map<String, ExternalFunction*, ltstr> FuncMap_t;

      FuncMap_t theFunctions;


    public:

      virtual ~Mongo2Module();

      virtual String
        getURI() const { return MONGO2_MODULE_NAMESPACE; }

      virtual zorba::ExternalFunction*
        getExternalFunction(const String& localName);

      virtual void destroy();

      static ItemFactory*
      getItemFactory()
      {
          return Zorba::getInstance(0)->getItemFactory();
      }

      static XmlDataManager*
      getXmlDataManager()
      {
        return Zorba::getInstance(0)->getXmlDataManager();
      }
  };

  typedef mongo::BSONObj mdb_conn;

  class MongoBaseFunction : public ContextualExternalFunction
{
      public:
        MongoBaseFunction(const Mongo2Module* module);

        virtual ~MongoBaseFunction();

        virtual String
          getURI() const;

    protected:
      const Mongo2Module* theModule;

      String
        getOneStringArgument(const Arguments_t&, int) const;

      Item
        getOneItemArgument(const Arguments_t&, int) const;

      void
        throwError(const char*, const char*) const;

      void
        isNotJSONError() const;

      mdb_conn*
        getInstance (const DynamicContext*, const String& aIdent) const;
};

  /*********************************************************/



  //this class is used to store the instance/connection object
  //stored within a dictionary/map with a string/uuid as key
  //taken from couchbase.h

  class InstanceMap : public ExternalFunctionParameter
  {
    private:
      typedef std::map<String, mdb_conn*> InstanceMap_t;
      InstanceMap_t* instanceMap;

    public:
      InstanceMap();

      bool
      storeInstance(String&, mdb_conn*);

      mdb_conn*
      getInstance(const String&);

      bool
      deleteInstance(String&);

      virtual void
      destroy() throw()
      {
        if (instanceMap)
        {
          for (InstanceMap_t::const_iterator lIter = instanceMap->begin();
               lIter != instanceMap->end(); ++lIter)
          {
            //TODO: destroy mongo db conn necessary?
            //lcb_destroy(lIter->second);
          }
          instanceMap->clear();
          delete instanceMap;
        }
        delete this;
      };

  };


  class Mongo2ItemSequence : public ItemSequence
  {
  public:
    class Mongo2Iterator : public Iterator
    {
    protected:
      zorba::ItemFactory* itemFactory;
      mongo::DBClientCursor* resultPtr;
      bool _isOpenVal;
      int fcount;
      std::auto_ptr<mongo::DBClientCursor>  _resultCursor;

      bool
      getItemFromBSONObj(mongo::BSONObj bsonObj, zorba::Item &aItem);

      bool
      getItemFromBSONElement(mongo::BSONElement elemValue, zorba::Item &valItem);


    public:
      Mongo2Iterator(std::auto_ptr<mongo::DBClientCursor> resultCursor):
        _isOpenVal(false), _resultCursor(resultCursor) {}

      virtual ~Mongo2Iterator(){}

      void open();

      bool next(zorba::Item &aItem);

      void close();

      bool isOpen() const { return _isOpenVal; }
    };

  protected:
    std::auto_ptr<mongo::DBClientCursor> resultCursor;

  public:
    Mongo2ItemSequence(std::auto_ptr<mongo::DBClientCursor> resultCursor)
      : resultCursor(resultCursor)
      {}

    virtual ~Mongo2ItemSequence() {}

    zorba::Iterator_t
      getIterator() { return new Mongo2Iterator(resultCursor); }
  };

  class Mongo2ItemSequence5 : public ItemSequence
  {
  public:
    class Mongo2Iterator5 : public Iterator
    {
    protected:
      std::vector<Item>*  _itms;
      std::vector<Item>::iterator _it;
      bool _isOpenVal;
      int fcount;

      bool
      getItemFromBSONObj(mongo::BSONObj bsonObj, zorba::Item &aItem);

      bool
      getItemFromBSONElement(mongo::BSONElement elemValue, zorba::Item &valItem);

      

    public:
      Mongo2Iterator5(std::vector<Item>* itms):
        _isOpenVal(false), _itms(itms) {}

      virtual ~Mongo2Iterator5(){}

      void open();

      bool next(zorba::Item &aItem);

      void close();

      bool isOpen() const { return _isOpenVal; }
    };

  protected:
    std::vector<Item>* _itms;


  public:
    Mongo2ItemSequence5(std::vector<Item>* itms)
      : _itms(itms)
      {}

    virtual ~Mongo2ItemSequence5() {}

    zorba::Iterator_t
      getIterator() { return new Mongo2Iterator5(_itms); }
  };


  /******************************************/

  std::string getTimestampStringFromSeconds(uint32_t sec);
  std::string getDateStringFromTimet(time_t tt);
  bool getItemFromBSONElement(ItemFactory *itemFactory, mongo::BSONElement elemValue, zorba::Item &valItem);
  bool getItemFromBSONObj(ItemFactory *itemFactory, mongo::BSONObj bsonObj, zorba::Item &aItem);

  class ConnectFunction : public MongoBaseFunction
  {
    public:
      ConnectFunction(const Mongo2Module* aModule)
        : MongoBaseFunction(aModule) {}

      virtual ~ConnectFunction(){}

      virtual zorba::String
        getLocalName() const { return "connect"; }

      virtual zorba::ItemSequence_t
        evaluate( const Arguments_t&,
                  const zorba::StaticContext*,
                  const zorba::DynamicContext*) const;
  };

  class QueryFunction : public MongoBaseFunction
  {
    public:
      QueryFunction(const Mongo2Module* aModule)
        : MongoBaseFunction(aModule) {}

      virtual ~QueryFunction(){}

      virtual zorba::String
        getLocalName() const { return "query"; }

      virtual zorba::ItemSequence_t
        evaluate( const Arguments_t&,
                  const zorba::StaticContext*,
                  const zorba::DynamicContext*) const;
  };

  class CountFunction : public MongoBaseFunction
  {
    public:
      CountFunction(const Mongo2Module* aModule)
        : MongoBaseFunction(aModule) {}

      virtual ~CountFunction(){}

      virtual zorba::String
        getLocalName() const { return "count"; }

      virtual zorba::ItemSequence_t
        evaluate( const Arguments_t&,
                  const zorba::StaticContext*,
                  const zorba::DynamicContext*) const;
  };


}

}


#endif //_COM_ZORBA_WWW_MODULES_MONGO2_H_
