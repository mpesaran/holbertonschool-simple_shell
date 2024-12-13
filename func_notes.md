# Function.c Notes

#### My code with detailed notes of why I have written what i've written. The brute force code i've trialed and errored. 😅

```
void print_prompt(void) {
        if (isatty(STDIN_FILEN0)) {
         printf("simple_shell$ ");
         fflush(stdout);
        }
}

// isatty       => checks the type of input source (text vs file input)
// STDIN        => standard INPUT represents the input from the users keyboard whilst running in a terminal. 
// fflush       => This function will "flush" the buffer and print immediately the data thats stored to its destination eg. 'printf'
```

