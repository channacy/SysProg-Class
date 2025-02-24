# !/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check clear runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check mkdir runs without errors" {
    run ./dsh <<EOF                
mkdir test1
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check rmdir runs without errors" {
    run ./dsh <<EOF                
rmdir test1
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check echo runs without errors" {
    run ./dsh <<EOF                
echo "hello world"
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="helloworlddsh2>dsh2>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check cd without arguments runs without errors" {
    run ./dsh <<EOF                
cd
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check cd with arguments runs without errors" {
    run ./dsh <<EOF                
cd ..
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check touch runs without errors" {
    run ./dsh <<EOF                
touch test.txt
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check rm runs without errors" {
    run ./dsh <<EOF                
rm test.txt
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check uname runs with Linux" {
    run ./dsh <<EOF                
uname
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="Linuxdsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check pwd outputs current directory" {
    run ./dsh <<EOF                
pwd
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="/home/channacyun/SysProg-Class/assignments/4-ShellP2/starterdsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check cd without arguments does not change current directory" {
    run ./dsh <<EOF
cd                
pwd
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="/home/channacyun/SysProg-Class/assignments/4-ShellP2/starterdsh2> dsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check cd with arguments does change current directory" {
    run ./dsh <<EOF
cd ..             
pwd
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="/home/channacyun/SysProg-Class/assignments/4-ShellP2dsh2> dsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check cd redirects to new directory" {
    run ./dsh <<EOF
cd /home/channacyun/SysProg-Class/assignments/4-ShellP2/starter/bats             
pwd
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="/home/channacyun/SysProg-Class/assignments/4-ShellP2/starter/batsdsh2> dsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check ls shows correct subdirectories after cd redirects to new directory" {
    run ./dsh <<EOF
cd /home/channacyun/SysProg-Class/assignments/4-ShellP2/starter/bats             
ls
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="assignment_tests.shstudent_tests.shdsh2> dsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check rmdir works after creating and listing directories" {
    run ./dsh <<EOF
mkdir test1            
rmdir test1
ls
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="batsdragon.cdshdsh_cli.cdshlib.cdshlib.hmakefileshell_roadmap.mddsh2> dsh2> dsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check rm works after creating and listing file" {
    run ./dsh <<EOF
touch test.txt            
rm test.txt
ls
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="batsdragon.cdshdsh_cli.cdshlib.cdshlib.hmakefileshell_roadmap.mddsh2> dsh2> dsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check whoami" {
    run ./dsh <<EOF
whoami
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="channacyundsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check hostname" {
    run ./dsh <<EOF
hostname
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="ubuntudsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check find after creating file" {
    run ./dsh <<EOF
touch test.txt   
find . -type f -name "*.txt"         
rm test.txt
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="./test.txtdsh2> dsh2> dsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "check find if no matches are found" {
    run ./dsh <<EOF
find . -type f -name "*.txt"         
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "It handles rm not executing when not provided an argument" {
    run "./dsh" <<EOF               
    rm
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="rm: missing operandTry 'rm --help' for more information.dsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Test displaying file permission" {
    run "./dsh" <<EOF               
    ls -ld shell_roadmap.md
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="-rw-r--r-- 1 channacyun channacyun 296 Feb 21 12:01 shell_roadmap.mddsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Test changing file permission" {
    run "./dsh" <<EOF
    chmod 444 shell_roadmap.md              
    ls -ld shell_roadmap.md
    chmod 644 shell_roadmap.md
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="-r--r--r-- 1 channacyun channacyun 296 Feb 21 12:01 shell_roadmap.mddsh2> dsh2> dsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}