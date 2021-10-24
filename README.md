# Probe-Tool

Probe

### Software requirements
## First Part:
During a space mission each rover of the team needs to collect some specific stones from the field. For this space mission, 2 input files are written in the following format. (field.txt and rovers.txt)

The first line of the "rovers.txt" file stores the number of rovers in the team.
Then, for each rover, the file stores:
- On the first line, its name (string of 10 characters at most),
Identifier (integer value), number of unique stone codes.
-On the second line, unique stone code (string of 10 characters at most), quality number of that unique stone.

The "field.txt" file contains characters representing the field, which contains some of the unique stone codes in it, that will be used to scan through to get the needed output. (6 rows, 20 columns).

Write a program that, after reading the files and storing its content in a proper data structure, output file will contain the following.
- First line of the output file stores the number of rovers in the team.
- For the each of the rover, there must be 2 lines which contains:
	-  Name, identifier (integer value), TOTAL number of stones collected.
	-  Next line: all quality numbers of the stones collected,
  	   stone_1 stone_2 ... stone_N, stored as real values.

## Second Part:
During a space mission each rover of the team
is checked after every time a stone collected.
For each rover all collected stone's quality numbers are stored in a file with the following
format.
The first line of the file stores the number of rovers in the team.
Then, for each rover, the file stores:
- On the first line, its name (string of 30 characters at most),
  identifier (integer value), and number of stones collected.
- On the second line, all quality numbers of the stones collected,
  stone_1 stone_2 ... stone_N, stored as real values.

Write a program that, after reading the file and storing its content
in a proper data structure, is able to reply to the following menu
inquiry:
- A command to list: the program prints-out the number of rovers, their names,
  identifiers, and number of stones collected.
- A command to show details of selected rover's name: given a rover name, the program prints-out
  its identifier, and all stone datas.
- A command to show the rover with highest average quality number: the program prints-out the name, identifier, all stone datas
  and the average stone quality for the rover whose average stone quality is
  bigger.
- A command to end the program.

Notice that all operations can be performed more than once till the
end command is issued.


## Info for recruitement

You can write this in any programming language,
try to complete the software as much as you can.

