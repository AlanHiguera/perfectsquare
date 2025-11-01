
#include "tipStack.h"
#include "Stack.h"

int main() {
    struct Stack* stack;
    struct tipStack* tipstackaux;
    
    tipstackaux = tipCreateStack(5);
    stack = createStack(6);
    
    tipPush(tipstackaux, 10);
    tipPush(tipstackaux, 20);
    tipPush(tipstackaux, 30);

    printf("print tipstack\n");
    tipPrintStack(tipstackaux);

    printf("pop tipstack: %u\n", tipPop(tipstackaux));

    tipPush(tipstackaux, 40);
    tipPush(tipstackaux, 50);

    printf("print tipstack2\n");    
    tipPrintStack(tipstackaux);
    
    llenarPila(stack, tipstackaux);
    push(stack, '1', 99); //agregamos un elemento mas para verificar que la pila funciona bien despues de llenar desde tipstack
    printf("Stack despues de llenar:\n");
    PrintStack(stack);


    printf("quitando todo de stack\n");
    struct ElementoPila elem;
    while(isEmpty(stack) != '0'){
        elem = pop(stack);
        printf("popped %u\n", elem.posicion);
    }

    tipFreeStack(tipstackaux);
    FreeStack(stack);
    return 0;
}
