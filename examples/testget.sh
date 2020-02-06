HTTP_COOKIE="cookie1=cookievalue1; cookie2=cookievalue2; cookie3=cookievalue3"
export HTTP_COOKIE

REQUEST_METHOD="GET"
export REQUEST_METHOD

CONTENT_TYPE=
export CONTENT_TYPE

QUERY_STRING="variable1=value1&variable2=value2&variable3=value3"
export QUERY_STRING

CONTENT_LENGTH="29"
export CONTENT_LENGTH

SCRIPT_NAME="/$1"
export SCRIPT_NAME

PATH_INFO="/default/testpages/htmlpage.html"
export PATH_INFO

DOCUMENT_ROOT="`pwd`"
export DOCUMENT_ROOT

SERVER_NAME="localhost.localdomain"
export SERVER_NAME

REMOTE_ADDR=
export REMOTE_ADDR

./$1
