## Assignment 2 Questions

#### Directions
Please answer the following questions and submit in your repo for the second assignment.  Please keep the answers as short and concise as possible.

1. In this assignment I asked you provide an implementation for the `get_student(...)` function because I think it improves the overall design of the database application.   After you implemented your solution do you agree that externalizing `get_student(...)` into it's own function is a good design strategy?  Briefly describe why or why not.

    > **Answer**: I agree externalizing `get_student(...)` into it's own function is a good design strategy because it enables you to access a student with a particular ID. The function is useful for other functions requiring the need to validate if a student exists before searching for the entry as well. If the functionality was coupled with another function (instead of having `get_student(...)`), this defeats the single responsibility rule. 

2. Another interesting aspect of the `get_student(...)` function is how its function prototype requires the caller to provide the storage for the `student_t` structure:

    ```c
    int get_student(int fd, int id, student_t *s);
    ```

    Notice that the last parameter is a pointer to storage **provided by the caller** to be used by this function to populate information about the desired student that is queried from the database file. This is a common convention (called pass-by-reference) in the `C` programming language. 

    In other programming languages an approach like the one shown below would be more idiomatic for creating a function like `get_student()` (specifically the storage is provided by the `get_student(...)` function itself):

    ```c
    //Lookup student from the database
    // IF FOUND: return pointer to student data
    // IF NOT FOUND: return NULL
    student_t *get_student(int fd, int id){
        student_t student;
        bool student_found = false;
        
        //code that looks for the student and if
        //found populates the student structure
        //The found_student variable will be set
        //to true if the student is in the database
        //or false otherwise.

        if (student_found)
            return &student;
        else
            return NULL;
    }
    ```
    Can you think of any reason why the above implementation would be a **very bad idea** using the C programming language?  Specifically, address why the above code introduces a subtle bug that could be hard to identify at runtime? 

    > **ANSWER:** The above implementation is not a good idea because it only allows you to return one value at a time and by allocating memory in the function itself, this could cause memory management overhead problems. With the implementation with pass-by-reference, you are able to return "multiple" values by returning one value and modifying the passed pointer into the parameter and modifying that value. Moreover, you won't be dynamically allocating memory and reduces the responsibility of the function. With this current example, a subtle bug could be introduced when the student cannot be found, and NULL is returned. When NULL is returned, it is hard to identify if there was a bug with opening or seeking the file or a bug with the student not being found. Furthermore, you would have to manage the memory after the value is returned to ensure there are no memory overhead issues.

3. Another way the `get_student(...)` function could be implemented is as follows:

    ```c
    //Lookup student from the database
    // IF FOUND: return pointer to student data
    // IF NOT FOUND or memory allocation error: return NULL
    student_t *get_student(int fd, int id){
        student_t *pstudent;
        bool student_found = false;

        pstudent = malloc(sizeof(student_t));
        if (pstudent == NULL)
            return NULL;
        
        //code that looks for the student and if
        //found populates the student structure
        //The found_student variable will be set
        //to true if the student is in the database
        //or false otherwise.

        if (student_found){
            return pstudent;
        }
        else {
            free(pstudent);
            return NULL;
        }
    }
    ```
    In this implementation the storage for the student record is allocated on the heap using `malloc()` and passed back to the caller when the function returns. What do you think about this alternative implementation of `get_student(...)`?  Address in your answer why it work work, but also think about any potential problems it could cause.  
    
    > **ANSWER:** This approach uses pstudent to store the possible student existing in the database and returns NULL if this the student with the specified id is not found. The implementation works because it updates the searched student into the pstudent structure instance as the file descriptor reads through the file. Some potential issues can arise when "fp", the file descriptor is not set to the correct position based on the calculated offset. Moreover, returning NULL does not indicate much about the error so adding log statements could provide better coding practices. Secondly, after student_found is set to True, and pstudent is returned; a possible issue of a memory leak could occur depending on how the value is handled. Other general memory management considerations are needed when dynamically allocated memory to the heap. 


4. Lets take a look at how storage is managed for our simple database. Recall that all student records are stored on disk using the layout of the `student_t` structure (which has a size of 64 bytes).  Lets start with a fresh database by deleting the `student.db` file using the command `rm ./student.db`.  Now that we have an empty database lets add a few students and see what is happening under the covers.  Consider the following sequence of commands:

    ```bash
    > ./sdbsc -a 1 john doe 345
    > ls -l ./student.db
        -rw-r----- 1 bsm23 bsm23 128 Jan 17 10:01 ./student.db
    > du -h ./student.db
        4.0K    ./student.db
    > ./sdbsc -a 3 jane doe 390
    > ls -l ./student.db
        -rw-r----- 1 bsm23 bsm23 256 Jan 17 10:02 ./student.db
    > du -h ./student.db
        4.0K    ./student.db
    > ./sdbsc -a 63 jim doe 285 
    > du -h ./student.db
        4.0K    ./student.db
    > ./sdbsc -a 64 janet doe 310
    > du -h ./student.db
        8.0K    ./student.db
    > ls -l ./student.db
        -rw-r----- 1 bsm23 bsm23 4160 Jan 17 10:03 ./student.db
    ```

    For this question I am asking you to perform some online research to investigate why there is a difference between the size of the file reported by the `ls` command and the actual storage used on the disk reported by the `du` command.  Understanding why this happens by design is important since all good systems programmers need to understand things like how linux creates sparse files, and how linux physically stores data on disk using fixed block sizes.  Some good google searches to get you started: _"lseek syscall holes and sparse files"_, and _"linux file system blocks"_.  After you do some research please answer the following:

    - Please explain why the file size reported by the `ls` command was 128 bytes after adding student with ID=1, 256 after adding student with ID=3, and 4160 after adding the student with ID=64? 

        > **ANSWER:** The file size increased to 4160 bytes after adding student with ID=64 because the `ls` command shows the actual number of bytes the file's size takes up including the data and holes (regions without written data). Thus, by adding another student with ID=3, the number of bytes naturally increased as the content does so.

    -   Why did the total storage used on the disk remain unchanged when we added the student with ID=1, ID=3, and ID=63, but increased from 4K to 8K when we added the student with ID=64? 

        > **ANSWER:**  In regards to sparse files, disk sizes are usually smaller than the file sizes because holes in the file does not take up disk space. In this example, the disk size was 4k. However, as more data was being added, when you use `lseek`, the file pointer can beyond the current end of the file and writes the data for the student with ID=64 in a "hole", a location of data with unallocated disk space. Thus, the operating system has to allocate new data to the previous empty section and results in the increase of disk space from 4K to 8K. Also, note the disk only allocates space for data that has been written or empty blocks but not for holes. 

    - Now lets add one more student with a large student ID number  and see what happens:

        ```bash
        > ./sdbsc -a 99999 big dude 205 
        > ls -l ./student.db
        -rw-r----- 1 bsm23 bsm23 6400000 Jan 17 10:28 ./student.db
        > du -h ./student.db
        12K     ./student.db
        ```
        We see from above adding a student with a very large student ID (ID=99999) increased the file size to 6400000 as shown by `ls` but the raw storage only increased to 12K as reported by `du`.  Can provide some insight into why this happened?

        > **ANSWER:**  The file size increases as data is being added so when the very large student ID was written into the file, the file size will always grow. (File size shows actual size of the overall file). However, the disk size does not nearly grow as large because it does not account for holes and only considers actual writen data. (Sparse files record where the blocks of 0s are located instead of actually remembering them.) However, if parts of the data of the new student is being written into the holes, the disk size will grow only for those memory blocks. This also suggests the file has a lot of unallocated space not being recorded in the disk. File size accounts for more blocks of the file, so file size usually increases more than the raw storage in a disk. These reasons explain the file size of 6400000 in comparison to the disk size increase to 12K after the very large student ID (ID=99999) was added.