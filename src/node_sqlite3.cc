#include <stdint.h>
#include <sstream>
#include <cstring>
#include <string>
#include <sqlite3.h>

#include "macros.h"
#include "database.h"
#include "statement.h"
#include "backup.h"

using namespace node_sqlite3;

static void simple_rank(sqlite3_context *pCtx, int nVal, sqlite3_value **apVal)
{

    unsigned int *aMatchinfo; /* Return value of matchinfo() */
    int nCol;                 /* Number of columns in the table */
    int nPhrase;              /* Number of phrases in the query */
    int iPhrase;              /* Current phrase */
    double score = 0.0;       /* Value to return */
    double defaultWeight = 1.0;

    assert(sizeof(int) == 4);

    /* Check that the number of arguments passed to this function is correct.
	** If not, jump to wrong_number_args. Set aMatchinfo to point to the array
	** of unsigned integer values returned by FTS function matchinfo. Set
	** nPhrase to contain the number of reportable phrases in the users full-text
	** query, and nCol to the number of columns in the table.
	*/
    aMatchinfo = (unsigned int *)sqlite3_value_blob(apVal[0]);
    nPhrase = aMatchinfo[0];
    nCol = aMatchinfo[1];
    if ((nVal - 1) > nCol)
        goto wrong_number_args;

    /* Iterate through each phrase in the users query. */
    for (iPhrase = 0; iPhrase < nPhrase; iPhrase++)
    {
        int iCol; /* Current column */

    /* Now iterate through each column in the users query. For each column,
	** increment the relevancy score by:
	**
	** (<hit count> / <global hit count>) * <column weight>
	**
	** aPhraseinfo[] points to the start of the data for phrase iPhrase. So
	** the hit count and global hit counts for each column are found in
	** aPhraseinfo[iCol*3] and aPhraseinfo[iCol*3+1], respectively.
	*/
        unsigned int *aPhraseinfo = &aMatchinfo[2 + iPhrase * nCol * 3];
        for (iCol = 0; iCol < nCol; iCol++)
        {
            int nHitCount = aPhraseinfo[3 * iCol];
            int nGlobalHitCount = aPhraseinfo[3 * iCol + 1];
            double weight = defaultWeight;
            if ((1 + iCol) < nVal)
                weight = sqlite3_value_double(apVal[1 + iCol]);
            if (nHitCount > 0)
            {
                score += ((double)nHitCount / (double)nGlobalHitCount) * weight;
            }
        }
    }

    sqlite3_result_double(pCtx, score);
    return;

    /* Jump here if the wrong number of arguments are passed to this function */
wrong_number_args:
    sqlite3_result_error(pCtx, "wrong number of arguments to function rank()", -1);
}

static int register_c74_extensions(
    sqlite3 *db,
    char **pzErrMsg,
    const sqlite3_api_routines *pThunk)
{

    sqlite3_create_function(db, "simple_rank", -1, SQLITE_ANY, 0, simple_rank, 0, 0);

    return SQLITE_OK;
}

namespace {

NAN_MODULE_INIT(RegisterModule) {
    Nan::HandleScope scope;

    Database::Init(target);
    Statement::Init(target);
    Backup::Init(target);

    DEFINE_CONSTANT_INTEGER(target, SQLITE_OPEN_READONLY, OPEN_READONLY);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_OPEN_READWRITE, OPEN_READWRITE);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_OPEN_CREATE, OPEN_CREATE);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_OPEN_FULLMUTEX, OPEN_FULLMUTEX);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_OPEN_URI, OPEN_URI);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_OPEN_SHAREDCACHE, OPEN_SHAREDCACHE);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_OPEN_PRIVATECACHE, OPEN_PRIVATECACHE);
    DEFINE_CONSTANT_STRING(target, SQLITE_VERSION, VERSION);
#ifdef SQLITE_SOURCE_ID
    DEFINE_CONSTANT_STRING(target, SQLITE_SOURCE_ID, SOURCE_ID);
#endif
    DEFINE_CONSTANT_INTEGER(target, SQLITE_VERSION_NUMBER, VERSION_NUMBER);

    DEFINE_CONSTANT_INTEGER(target, SQLITE_OK, OK);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_ERROR, ERROR);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_INTERNAL, INTERNAL);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_PERM, PERM);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_ABORT, ABORT);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_BUSY, BUSY);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_LOCKED, LOCKED);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_NOMEM, NOMEM);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_READONLY, READONLY);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_INTERRUPT, INTERRUPT);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_IOERR, IOERR);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_CORRUPT, CORRUPT);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_NOTFOUND, NOTFOUND);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_FULL, FULL);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_CANTOPEN, CANTOPEN);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_PROTOCOL, PROTOCOL);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_EMPTY, EMPTY);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_SCHEMA, SCHEMA);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_TOOBIG, TOOBIG);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_CONSTRAINT, CONSTRAINT);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_MISMATCH, MISMATCH);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_MISUSE, MISUSE);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_NOLFS, NOLFS);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_AUTH, AUTH);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_FORMAT, FORMAT);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_RANGE, RANGE);
    DEFINE_CONSTANT_INTEGER(target, SQLITE_NOTADB, NOTADB);

    sqlite3_auto_extension((void (*)(void))register_c74_extensions);
}

}

const char* sqlite_code_string(int code) {
    switch (code) {
        case SQLITE_OK:         return "SQLITE_OK";
        case SQLITE_ERROR:      return "SQLITE_ERROR";
        case SQLITE_INTERNAL:   return "SQLITE_INTERNAL";
        case SQLITE_PERM:       return "SQLITE_PERM";
        case SQLITE_ABORT:      return "SQLITE_ABORT";
        case SQLITE_BUSY:       return "SQLITE_BUSY";
        case SQLITE_LOCKED:     return "SQLITE_LOCKED";
        case SQLITE_NOMEM:      return "SQLITE_NOMEM";
        case SQLITE_READONLY:   return "SQLITE_READONLY";
        case SQLITE_INTERRUPT:  return "SQLITE_INTERRUPT";
        case SQLITE_IOERR:      return "SQLITE_IOERR";
        case SQLITE_CORRUPT:    return "SQLITE_CORRUPT";
        case SQLITE_NOTFOUND:   return "SQLITE_NOTFOUND";
        case SQLITE_FULL:       return "SQLITE_FULL";
        case SQLITE_CANTOPEN:   return "SQLITE_CANTOPEN";
        case SQLITE_PROTOCOL:   return "SQLITE_PROTOCOL";
        case SQLITE_EMPTY:      return "SQLITE_EMPTY";
        case SQLITE_SCHEMA:     return "SQLITE_SCHEMA";
        case SQLITE_TOOBIG:     return "SQLITE_TOOBIG";
        case SQLITE_CONSTRAINT: return "SQLITE_CONSTRAINT";
        case SQLITE_MISMATCH:   return "SQLITE_MISMATCH";
        case SQLITE_MISUSE:     return "SQLITE_MISUSE";
        case SQLITE_NOLFS:      return "SQLITE_NOLFS";
        case SQLITE_AUTH:       return "SQLITE_AUTH";
        case SQLITE_FORMAT:     return "SQLITE_FORMAT";
        case SQLITE_RANGE:      return "SQLITE_RANGE";
        case SQLITE_NOTADB:     return "SQLITE_NOTADB";
        case SQLITE_ROW:        return "SQLITE_ROW";
        case SQLITE_DONE:       return "SQLITE_DONE";
        default:                return "UNKNOWN";
    }
}

const char* sqlite_authorizer_string(int type) {
    switch (type) {
        case SQLITE_INSERT:     return "insert";
        case SQLITE_UPDATE:     return "update";
        case SQLITE_DELETE:     return "delete";
        default:                return "";
    }
}

NODE_MODULE(node_sqlite3, RegisterModule)
