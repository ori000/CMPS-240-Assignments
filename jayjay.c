#include <stdio.h>
#include <stdint.h>

// Function to check if a number is a JAYJAY number
size_t isJAYJAY(size_t n) {
    int count[4] = {0, 0, 0, 0};

    if (n < 1)
        return 0;
    else if (n == 1)
        return 1;

    while (n > 0) {
        size_t digit = n % 10;

        if (digit != 1 && digit != 2 && digit != 3) {
            return 0;
        }
        else {
            ++count[digit];
            n /= 10;
        }
    }
    if( (isJAYJAY(count[1]) || count[1] == 0) && 
        (isJAYJAY(count[2]) || count[2] == 0) && 
        (isJAYJAY(count[3]) || count[3] == 0) ) {
        return 1;
    }
    else 
        return 0;
}

size_t calculateJAYJAY(size_t current){
    while (1) {
        current++;
        if (isJAYJAY(current)) {
            return current;
        }
    }
}

size_t JAYJAY(size_t K){

    size_t answer = 0;
    size_t current = 0;
    size_t T[K]; // Store JAYJAY numbers up to K

    // Calculate JAYJAY numbers up to K
    for (size_t i = 1; i <= K; i++) {
        current = calculateJAYJAY(current);
        T[i] = current;
    }
    answer = T[K];
    return answer;
}

int main() {
    printf("%ld\n",isJAYJAY(111111111111));

    int i = 1000;
    int index = i;
    
    printf("%dth JAY JAY number is %d\n", index, JAYJAY(i));
    printf("%dth JAY JAY number mod 321321321 is %d", index, JAYJAY(i) % 321321321);

    return 0;
}
