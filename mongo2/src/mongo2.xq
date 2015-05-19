xquery version "3.0";

module namespace mongo2 = "http://www.zorba-xquery.com/modules/mongo2";

declare namespace jn = "http://jsoniq.org/functions";
declare namespace an = "http://zorba.io/annotations";

declare namespace ver = "http://zorba.io/options/versioning";
declare option ver:module-version "1.0";


declare %an:nondeterministic function mongo2:query($connString as object(), $collection as xs:string, $query as xs:string)
  as object()* external;

declare %an:nondeterministic function mongo2:count($connString as object(), $collection as xs:string, $query as xs:string)
  as xs:integer external;
