
#include <stdio.h>
#include <stdint.h>


typedef void (*call_ctor_t)(void);
extern call_ctor_t __ctors_start__[];
extern call_ctor_t __ctors_end__[];


void my_custom_startup() {
    call_ctor_t * ctor_call = __ctors_start__;
    while ((ctor_call < __ctors_end__)&&((unsigned int)*ctor_call!=0xFFFFFFFF)&&((unsigned int)*ctor_call!=0x00000000)) {
        (*ctor_call)();
        ctor_call++;
    }
}


class testClass {


public:

    float pp = 20;
    testClass(float p = 0) : pp(p) {
        printf("testclass constructor called");
    }

    float method(){
        return 6.0f;
    };
};




testClass staticTest(99);


extern "C" int main(){

};


extern "C" int local_main(int arg) {

    testClass* obj = new testClass();

    my_custom_startup();
    printf("this print statement was hot loaded via fros \n");
    printf("It works! %f\n", obj->method());
    // return ftv();
    return 0;
}
