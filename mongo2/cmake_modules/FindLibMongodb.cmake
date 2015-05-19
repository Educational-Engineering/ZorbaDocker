if(MONGO_INCLUDE_DIR AND MONGO_LIBRARIES)
   set(MONGO_FOUND TRUE)

else(MONGO_INCLUDE_DIR AND MONGO_LIBRARIES)

	find_path(MONGO_INCLUDE_DIR client/dbclient.h
	  /usr/include/mongo
	  /usr/local/include/mongo
	  /opt/local/include/mongo
    )

  find_library(MONGO_LIBRARIES NAMES mongoclient  libmongoclient
    PATHS
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    )

  if(MONGO_INCLUDE_DIR AND MONGO_LIBRARIES)
    set(MONGO_FOUND TRUE)
    message(STATUS "Found MongoDB: ${MONGO_INCLUDE_DIR}, ${MONGO_LIBRARIES}")
    INCLUDE_DIRECTORIES(${MONGO_INCLUDE_DIR})
  else(MONGO_INCLUDE_DIR AND MONGO_LIBRARIES)
    set(MONGO_FOUND FALSE)
    message(STATUS "MongoDB not found.")
  endif(MONGO_INCLUDE_DIR AND MONGO_LIBRARIES)

mark_as_advanced(MONGO_INCLUDE_DIR MONGO_LIBRARIES)
  mark_as_advanced(MONGO_VERSION_CHECK)

endif(MONGO_INCLUDE_DIR AND MONGO_LIBRARIES)

if (MONGO_FOUND)
  find_path(MONGO_VERSION_CHECK util/net/hostandport.h
  	/usr/include/mongo/
  	/usr/local/include/mongo/
	/opt/local/include/mongo)

  if (MONGO_VERSION_CHECK)
     set (MONGO_VERSION_2 TRUE)
  endif(MONGO_VERSION_CHECK)
endif (MONGO_FOUND)
