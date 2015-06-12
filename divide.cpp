#include <my_global.h>
#include <my_sys.h>
#include <mysql_com.h>

#include <new>
#include <vector>
#include <algorithm>

#if defined(MYSQL_SERVER)
#include <m_string.h>		/* To get strmov() */
#else
/* when compiled as standalone */
#include <string.h>
#define strmov(a,b) stpcpy(a,b)
#endif

#include <mysql.h>
#include <ctype.h>

#ifdef _WIN32
/* inet_aton needs winsock library */
#pragma comment(lib, "ws2_32")
#pragma warning
#endif

#ifdef HAVE_DLOPEN

#if !defined(HAVE_GETHOSTBYADDR_R) || !defined(HAVE_SOLARIS_STYLE_GETHOST)
static pthread_mutex_t LOCK_hostname;
#endif

C_MODE_START
my_bool divide_init(UDF_INIT *initid,UDF_ARGS *args,char *message);
double divide(UDF_INIT *initid,UDF_ARGS *args,char *is_null,char *error);
C_MODE_END

my_bool divide_init(UDF_INIT *initid,UDF_ARGS *args,char *message){
    if(args->arg_count != 2){
        strcpy(message,"divide() require exactly 2 arguments");
        return 1;
    }
    if(args->arg_type[0] == STRING_RESULT || args->arg_type[1] == STRING_RESULT){ 
        strcpy(message,"divede() require two real number"); 
        return 1;
    }
    args->arg_type[0] = REAL_RESULT;
    args->arg_type[1] = REAL_RESULT;
    initid->decimals = NOT_FIXED_DEC;
    initid->max_length = initid->decimals + 13;
    return 0;
}

double divide(UDF_INIT *initid,UDF_ARGS *args,char *is_null,char *error){
    if(!args->args[0]||!args->args[1]){
        *is_null = 1;
    }
    double dividend = *((double*)args->args[0]),divisor = *((double*)args->args[1]);
    if(divisor == 0){
        return 0;
    }else{
        return dividend/divisor;
    }
}

#endif
