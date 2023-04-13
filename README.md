# tempo-detection-eval

This repo has two apps : `musehub-file-decoder` and `tempo-detection-eval`.

## musehub-file-decoder

MuseHub files are Opus files, automatically decoded to wav when drag-n-dropping.

The Opus headers have tempo + key information. The BPM info tokenized is some of the file names is alledgedly not consistently there.

`musehub-file-decoder` decodes all opus files in the provided directory to wav files, renaming them `bpm=<bpm value or "none">_key=<key value or "none">__<original file name>.wav`.

That way the files can conveniently be played back and their bpm value reliably read from the file name by `tempo-detection-eval`.

## tempo-detection-eval

Reads all files in the directory argument, for each evaluated solution (at the time of writing only [MiniBpm](https://github.com/breakfastquay/minibpm)) runs the tempo estimate. The results (actual, expected, error, false-positive, false-negative, etc) are written to `tempo-detection-eval.csv` in the working directory.

# Brainstorming on product behaviour upon clip import

Legend:

| column  | values                  | description                                   |
| :------ | ----------------------- | --------------------------------------------- |
| tempo   | none, fixed or changing |                                               |
| is loop | 0 or 1                  | if loop, clip begins and ends exactly on beat |
| is 1st  | 0 or 1                  | 1st clip with tempo in the project            |

| tempo           | is loop | is 1st | auto-stretch ? | sets project tempo ? | Comments                                                             |
| :-------------- | ------- | ------ | -------------- | -------------------- | -------------------------------------------------------------------- |
| none / changing | N/A     | 0      | no             | no                   | changing-tempo not supported -> treat as without tempo               |
| none / changing | N/A     | 1      | no             | no                   |                                                                      |
| fixed           | 0       | 0      | yes            | no                   | Auto-stretching isn't correct, but loop detection is much more work. |
| fixed           | 0       | 1      | yes            | yes                  | as above                                                             |
| fixed           | 1       | 0      | yes            | no                   |                                                                      |
| fixed           | 1       | 1      | yes            | yes                  |                                                                      |

# Library requirements

Candidate solution criteria :

- open-source
- can tell if tempo or not tempo
- categorizes audio with changing tempo as no-tempo (e.g. float or bool confidence value)
