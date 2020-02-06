HTTP_COOKIE="cookievar1=cookieval1; cookievar2=cookieval2; cookievar3=cookieval3"
export HTTP_COOKIE

REQUEST_METHOD="POST"
export REQUEST_METHOD

CONTENT_TYPE="application/x-www-form-urlencoded"
export CONTENT_TYPE

CONTENT_LENGTH="29"
export CONTENT_LENGTH

SCRIPT_NAME="/application/modules/module/cgi.cgi"
export SCRIPT_NAME

DOCUMENT_ROOT="`pwd`"
export DOCUMENT_ROOT

SERVER_NAME="localhost.localdomain"
export SERVER_NAME

REMOTE_ADDR=
export REMOTE_ADDR

echo "var1=val1&var2=val2&var3=val3" | ./cgi.cgi
