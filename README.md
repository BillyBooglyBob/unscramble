# Word Unscrambler

This program allows users to make words from a given set of letters within a specified range of lengths.

## Usage

You can provide up to six arguments to the file:

- `--letters <value>`: Letters used to play the game.
- `--min-length <value>`: Minimum length of the word.
- `--dict <value>`: Directory for the list of words to use as the dictionary of correct words.

**Note:** Enter `Ctrl + D` to exit the game.

### Example Usages (**<text>** are user input)

#### Example 1 (No Arguments):
```
$ make unscramble
$ ./unscrambleWelcome to Word Unscrambler!
Enter words of length 3 to 7 made from the letters "ZFEFXVK"
**vx**
Word too short - it must be at least 3 characters long
**Ctrl+D**
No words guessed!
```

#### Example 2 (With Arguments):
```
$ make unscramble
$ ./unscramble --letters abcdefg --min-length 3
Welcome to Word Unscrambler!
Enter words of length 3 to 7 made from the letters "abcdefg"
bag
OK! Score so far is 3
**Ctrl+D**
Your final score is 3
```
