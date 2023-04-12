# tempo-detection-eval

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

## Library requirements

- open-source
- can tell if tempo or not tempo
- categorizes audio with changing tempo as no-tempo (e.g. float or bool confidence value)
