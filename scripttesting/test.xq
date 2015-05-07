jsoniq version "1.0";
import module namespace mongo2="http://www.zorba-xquery.com/modules/mongo2";


mongo2:query("127.0.0.1:27017", "testqueries.tq2", "{ age :{$gt: 3}} ")
