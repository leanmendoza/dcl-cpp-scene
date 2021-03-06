#include "quickjs.h"
#include <string>
#include <assert.h>

int quickjs_test(std::string address, int fdDebuggerInput, int fdDebuggerOutput) {
    auto runtime = JS_NewRuntime();
    auto ctx = JS_NewContext(runtime);

    if (fdDebuggerInput != 0){ 
        std::string QUICKJS_DEBUG_ADDRESS = address + "\n" + 
                "fdin:" + std::to_string(fdDebuggerInput) + "\n"
                "fdout:" + std::to_string(fdDebuggerOutput) + "\n";

        setenv("QUICKJS_DEBUG_ADDRESS", QUICKJS_DEBUG_ADDRESS.c_str(), 1);
    }

    std::string code = R"(
        var a = 7, b = 5;
        `This is a string from quickjs where a+b=${a+b}`
    )";

	JSValue result = JS_Eval(ctx, code.c_str(), code.length(), "<evalScript>", JS_EVAL_TYPE_GLOBAL);
    const char* resultString = JS_ToCString(ctx, result);

	if (JS_IsException(result)) {
		JSValue realException = JS_GetException(ctx);
		auto errorMessage = JS_ToCString(ctx, realException);
        return -1;
	}

    if (resultString){
        printf("QuickJS result: '%s'\n", resultString);
    }else{
        printf("QuickJS result: NULL\n", resultString);
    }

	// JSValue json = JS_JSONStringify(ctx, result, JS_UNDEFINED, JS_UNDEFINED);
	JS_FreeValue(ctx, result);
    free((void*)resultString);
//    assert(result == 12);

    return 0;
}
