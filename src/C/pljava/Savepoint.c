/*
 * Copyright (c) 2004, 2005 TADA AB - Taby Sweden
 * Distributed under the terms shown in the file COPYRIGHT
 * found in the root folder of this project or at
 * http://eng.tada.se/osprojects/COPYRIGHT.html
 *
 * @author Thomas Hallgren
 */
#include <postgres.h>
#include <executor/spi.h>
#include <executor/tuptable.h>

#include "org_postgresql_pljava_internal_Savepoint.h"
#include "pljava/Exception.h"
#include "pljava/type/String.h"
#include "pljava/SPI.h"

extern void Savepoint_initialize(void);
void Savepoint_initialize(void)
{
	JNINativeMethod methods[] =
	{
		{
		"_set",
	  	"(Ljava/lang/String;)J",
	  	Java_org_postgresql_pljava_internal_Savepoint__1set
		},
		{
		"_release",
		"(J)V",
		Java_org_postgresql_pljava_internal_Savepoint__1release
		},
		{
		"_rollback",
		"(J)V",
		Java_org_postgresql_pljava_internal_Savepoint__1rollback
		},
		{
		"_getName",
		"(J)Ljava/lang/String;",
		Java_org_postgresql_pljava_internal_Savepoint__1getName
		},
		{ 0, 0, 0 }
	};
	PgObject_registerNatives("org/postgresql/pljava/internal/Savepoint", methods);
}

/****************************************
 * JNI methods
 ****************************************/
/*
 * Class:     org_postgresql_pljava_internal_Savepoint
 * Method:    _set
 * Signature: (Ljava/lang/String;)J;
 */
JNIEXPORT jlong JNICALL
Java_org_postgresql_pljava_internal_Savepoint__1set(JNIEnv* env, jclass cls, jstring jname)
{
	jlong result = 0;
	BEGIN_NATIVE
	PG_TRY();
	{
		Ptr2Long p2l;
		char* name = String_createNTS(jname);
		MemoryContext currCtx = MemoryContextSwitchTo(JavaMemoryContext);
		p2l.ptrVal = SPI_setSavepoint(name);
		result = p2l.longVal;
		MemoryContextSwitchTo(currCtx);
		pfree(name);
	}
	PG_CATCH();
	{
		Exception_throw_ERROR("SPI_setSavepoint");
	}
	PG_END_TRY();
	END_NATIVE
	return result;
}

/*
 * Class:     org_postgresql_pljava_internal_Savepoint
 * Method:    _getName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_postgresql_pljava_internal_Savepoint__1getName(JNIEnv* env, jclass clazz, jlong _this)
{
	jstring result = 0;
	if(_this != 0)
	{
		BEGIN_NATIVE
		Ptr2Long p2l;
		p2l.longVal = _this;
		result = String_createJavaStringFromNTS(((Savepoint*)p2l.ptrVal)->name);
		END_NATIVE
	}
	return result;
}

/*
 * Class:     org_postgresql_pljava_internal_Savepoint
 * Method:    _release
 * Signature: (J)V
 */
JNIEXPORT void JNICALL
Java_org_postgresql_pljava_internal_Savepoint__1release(JNIEnv* env, jclass clazz, jlong _this)
{
	if(_this != 0)
	{
		BEGIN_NATIVE
		Ptr2Long p2l;
		p2l.longVal = _this;
		PG_TRY();
		{
			SPI_releaseSavepoint((Savepoint*)p2l.ptrVal);
		}
		PG_CATCH();
		{
			Exception_throw_ERROR("SPI_releaseSavepoint");
		}
		PG_END_TRY();
		END_NATIVE
	}
}

/*
 * Class:     org_postgresql_pljava_internal_Savepoint
 * Method:    _rollback
 * Signature: (J)V
 */
JNIEXPORT void JNICALL
Java_org_postgresql_pljava_internal_Savepoint__1rollback(JNIEnv* env, jclass clazz, jlong _this)
{
	if(_this != 0)
	{
		BEGIN_NATIVE
		Ptr2Long p2l;
		p2l.longVal = _this;
		PG_TRY();
		{
			SPI_rollbackSavepoint((Savepoint*)p2l.ptrVal);
		}
		PG_CATCH();
		{
			Exception_throw_ERROR("SPI_rollbackSavepoint");
		}
		PG_END_TRY();
		END_NATIVE
	}
}
