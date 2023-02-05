#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    int num1 = 14;
    int num2 = 0;
    int param2 = 0;
    int param3 = 14;
    int ret;
    ret = func4(num1,param2,param3,num2);

    printf("Returned value is: %d\n", ret);
    return 0;
}


int func4(int num_1, int param2, int param3, int num_2){
    int result;
    result = param3 - param2;
    int most_sig = (result <= 0);
    result = result + most_sig;
    result = result >> 1;
    num_2 = result + param2;

    if (num_2 <= num_1){
        result = 0;
        if (num_2 >= num_1){
            return result;
        }
        param2 = num_2 + 1;
        result = func4(num_1,param2,param3,num_2);
        return result = 1+2*result;
    }
    else{
        param3 = num_2-1;
        result = func4(num_1,param2,param3,num_2);
        return result = 2*result;
    }
}