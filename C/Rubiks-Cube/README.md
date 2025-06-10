# Rubik's Cube Program

*—The Polish version of the description is available directly inside the `RUBIK.c` file.—*

A program that simulates and manipulates a Rubik's Cube of size N x N x N.

### Program Flow:

1. The program reads a series of commands from the user to rotate the cube.

2. Input instructions should follow the format specified below.

3. It processes and executes each command in sequence.

4. When the program encounters a "print cube" command, it displays the cube's current state.

### Input Data Format

Instructions should be given as a sequence of commands, ending with a period (`.`). The program ignores any input after the period.

There are two types of commands: **rotation** and **print**.

#### Rotation Command

A rotation command has the structure: `[layers][face][angle]`

* **`[layers]` (Optional):** A positive integer specifying how many layers to rotate, starting from the designated face.

  * If this part is omitted, it defaults to **1** layer.

  * *Example:* `2f` means rotating the two front layers.

* **`[face]` (Required):** A single character that specifies the face to rotate.

  * `u`: up

  * `l`: left

  * `f`: front

  * `r`: right

  * `b`: back

  * `d`: down

* **`[angle]` (Optional):** A character indicating the angle and direction of rotation (when looking at the specified face directly).

  * If omitted: **+90°** rotation (clockwise).

  * `'` (apostrophe): **-90°** rotation (counter-clockwise).

  * `"` (double quote): **180°** rotation.

#### Print Command

* A **newline character** (`\n`) is a command to print the current state of the cube to the console.

#### Command Examples

* `r`: Rotates the single rightmost layer by 90° clockwise.

* `3u'`: Rotates the top three layers by 90° counter-clockwise.

* `2b"`: Rotates the two back layers by 180°.

* `fu'r\n.` : A sequence that rotates the front face, then the top face, then the right face, prints the cube's state, and finally ends the program.

---

**Author:** Kamila Paulina Godlewska [kamapago@gmail.com](mailto:kamapago@gmail.com)

**Version:** 1.0

**Date:** 05.12.2024

<br>

### How to Test This Program:

The easiest way to see this program in action is to:

1. Download the whole folder.

2. Unpack it.

3. Use the `testuj.sh` script to run the tests provided in the "testy" directory:

   ```bash
   ./testuj.sh RUBIK.c testy
   ```

---

### Author's Rights:

This program was built as an academic project. Some of the tests inside the `testy` directory were written by my professors, so I don't take credit for all of them. Similarly, the `testuj.sh` script is an adaptation of Tomasz Waleń's program, which was shared with us during classes.
