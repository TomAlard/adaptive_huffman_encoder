# Compression test results

### Uncompressed file sizes

| files                | KiB  |
|----------------------|------|
| alphabet.txt         | 98   |
| bible.txt            | 3953 |
| gutenberg_psyche.txt | 1977 |
| kennedy.xls          | 1006 |
| lcet10.txt           | 417  |
| plrabn12.txt         | 471  |
| ptt5                 | 502  |
| random.txt           | 98   |
 

### Compression results for Ordinary Adaptive Huffman (LEN = 1)

The MEM argument has no effect on any of these values, as Ordinary Adaptive Huffman
uses very little memory.

| files                | KiB     |
|----------------------|---------|
| alphabet.txt         | 58.74   |
| bible.txt            | 2166.63 |
| gutenberg_psyche.txt | 1211.16 |
| kennedy.xls          | 452.47  |
| lcet10.txt           | 244.91  |
| plrabn12.txt         | 269.35  |
| ptt5                 | 104.42  |
| random.txt           | 73.58   |

### Compression results for LEN = 9

The columns represent the value passed to the MEM argument. All values in KiB.
'-' means that this value is identical to the previous one in the row.

|                      | 0       | 1       | 2       | 3       | 4       | 5       | 6 | 7 | 8 | 9 |
|----------------------|---------|---------|---------|---------|---------|---------|---|---|---|---|
| alphabet.txt         | 56.0    | -       | -       | 8.0     | 6.25    | 6.71    | - | - | - | - |
| bible.txt            | 1964.0  | 1908.0  | 1888.0  | 1676.0  | 1378.62 | 1324.33 | - | - | - | - |
| gutenberg_psyche.txt | 1137.13 | 1103.14 | 1092.17 | 1026.87 | 911.14  | 902.01  | - | - | - | - |
| kennedy.xls          | 468.0   | 404.0   | -       | 323.52  | 325.08  | 314.73  | - | - | - | - |
| lcet10.txt           | 225.79  | 221.75  | 222.11  | 206.29  | 189.9   | 185.0   | - | - | - | - |
| plrabn12.txt         | 247.13  | 243.62  | 245.05  | 233.44  | 216.09  | 213.34  | - | - | - | - |
| ptt5                 | 76.0    | -       | -       | -       | 73.04   | 74.85   | - | - | - | - |
| random.txt           | 72.0    | -       | -       | -       | 80.84   | 84.43   | - | - | - | - |