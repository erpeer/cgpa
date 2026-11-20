# GPA / CGPA Transcript Management CLI (C++)

A command-line C++ application for managing student courses, calculating total credits and CGPA, sorting courses by grade, searching by course code, and saving transcripts to `.txt` and `.csv` formats.

## Project Structure

- `main.cpp` — Interactive CLI menu, grade validation, existing transcript loading (`transcript_<ID>.txt` / `transcript_<ID>.csv`), and `.txt`/`.csv` export.
- `student.h` & `student.cpp` — `Student` class managing student profile info, course list, total credits, CGPA calculation, grade sorting, code search, and console transcript printing.
- `course.h` & `course.cpp` — `Course` class representing course code, course name, letter grade, credit, grade points, and weighted points.
- `letter_convert.txt` — Grade point scale reference (`A` to `E`).
- `.gitignore` — Excludes compiled binaries, transcript output files, and local IDE settings.

## Grade Scale (`letter_convert.txt`)

| Letter Grade | Grade Points |
| :----------- | :----------- |
| `A`          | `4.00`       |
| `AB`         | `3.50`       |
| `B`          | `3.00`       |
| `BC`         | `2.50`       |
| `C`          | `2.00`       |
| `D`          | `1.00`       |
| `E`          | `0.00`       |

## Features

1. **Add Course** — Validates letter grade and positive credit values, and prevents duplicate course codes.
2. **Sort Courses by Grade** — Sorts courses from highest to lowest grade points.
3. **Search Course by Code** — Looks up a course by its code.
4. **Print Transcript (Console)** — Displays formatted course rows, total credits, and CGPA in the terminal.
5. **Save Transcript (`.txt`)** — Exports/updates `transcript_<ID>.txt` formatted for text editors/Notepad.
6. **Save Transcript (`.csv`)** — Exports/updates `transcript_<ID>.csv` formatted for spreadsheet tools such as Excel and Google Sheets.

## Build & Run

Compile from the project directory using `g++`:

```bash
g++ main.cpp student.cpp course.cpp -o cgpacalcu.exe
./cgpacalcu.exe
```
