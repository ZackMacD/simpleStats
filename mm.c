#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#define debug 0

// Comparison function for qsort()
int numcmp (const void *a, const void *b) {
    int x = *((int*) a);
    int y = *((int*) b);
    if (x > y) return 1;
    if (x < y) return -1;
    return 0;
}

// Function to calculate the mean of a vector
int mean (int *v, int length) {
    int i;
    int sum = 0;
    for(i = 0; i < length; i++) {
        sum = sum + v[i];
    }
    return sum/length;
}

int median (int *v, int length) {
    if (length%2 == 1)
        return v[(length/2)];
    else
        return ((v[(length/2)-1]+v[(length)/2])/2);
}

int main(int argc, char *argv[]) {

    int i, length, *pt;
    
    // Check for proper usage
    if (argc < 2) {
        fprintf(stderr, "%s: Aborting, not enough arguments.\n", argv[0]);
        return (-1);
    }

    // Determine amount of numbers from argc 
    length = argc - 1;
#if debug
    fprintf(stderr, "%s: DEBUG: %d numbers were passed.\n", argv[0], length);
#endif

    // Allocate memory for array of number (and error check)
    if ((pt = malloc(length * sizeof(int))) == NULL) {
        fprintf(stderr, "%s: Could not allocate memory.\n", argv[0]);
    }
        
    // Read numbers into array
    for (i = 0; i < length; i++) {
        pt[i] = (int) strtol(argv[i+1], NULL, 10);
    }

    // Sort numbers
    qsort(pt, length, sizeof(int), numcmp);

    int rc = fork();
    if (rc == -1) {
        // could not fork another process
        fprintf(stdout, "Fork failed.\n");
    exit(-1);
    } else if (rc == 0) { // child runs median:
        int med = median(pt, length);
        fprintf(stdout, "Median = %d \n", med);
    } else if (rc > 0) { // parent runs mean:
        wait(NULL); // waits until child has finished
        int avg = mean(pt, length);
        fprintf(stdout, "Mean = %d ", avg);

    }

    fprintf(stdout, "\n%s: FIN. \n", argv[0]);

    return 0;
}
