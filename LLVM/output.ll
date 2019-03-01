; ModuleID = 'dana program'
source_filename = "dana program"

%dummy = type opaque
%struct_main = type { %dummy*, [3 x [3 x i32]], i32, i32 }
%struct_isSymmetric = type { %struct_main*, [3 x [3 x i32]]*, i32, i32 }
%struct_isTriangular = type { %struct_main*, [3 x [3 x i32]]*, i32, i32, i32, i32 }

@0 = private constant [51 x i8] c"Please, give all elements of the matrix (3 by 3):\0A\00"
@1 = private constant [25 x i8] c"The matrix is symmetric\0A\00"
@2 = private constant [29 x i8] c"The matrix is not symmetric\0A\00"
@3 = private constant [30 x i8] c"The matrix is not triangular\0A\00"
@4 = private constant [32 x i8] c"The matrix is lower triangular\0A\00"
@5 = private constant [32 x i8] c"The matrix is upper triangular\0A\00"
@6 = private constant [53 x i8] c"The matrix is diagonal (lower and upper triangular)\0A\00"

declare void @writeInteger(i32)

declare void @writeChar(i8)

declare void @writeByte(i8)

declare void @writeString(i8*)

declare i32 @readInteger()

declare i8 @readChar()

declare i8 @readByte()

declare void @readString(i32, i8*)

declare i32 @extend(i8)

declare i8 @shrink(i32)

declare i32 @strlen(i8*)

declare i32 @strcmp(i8*, i8*)

declare void @strcpy(i8*, i8*)

declare void @strcat(i8*, i8*)

define void @main(%dummy* %previous) {
entry:
  %new_frame = alloca %struct_main
  %0 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 0
  store %dummy* %previous, %dummy** %0
  call void @writeString(i8* getelementptr inbounds ([51 x i8], [51 x i8]* @0, i32 0, i32 0))
  %1 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  store i32 0, i32* %1
  br label %loop

return:                                           ; preds = %endif36
  ret void

loop:                                             ; preds = %endif10, %entry
  %2 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  store i32 0, i32* %2
  br label %loop1

after_loop:                                       ; preds = %then9
  %3 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  store i32 0, i32* %3
  br label %loop12

loop1:                                            ; preds = %endif, %loop
  %4 = call i32 @readInteger()
  %5 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  %j = load i32, i32* %5
  %6 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %i = load i32, i32* %6
  %7 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 1
  %8 = getelementptr inbounds [3 x [3 x i32]], [3 x [3 x i32]]* %7, i32 0, i32 %i
  %9 = getelementptr inbounds [3 x i32], [3 x i32]* %8, i32 0, i32 %j
  store i32 %4, i32* %9
  %10 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  %j3 = load i32, i32* %10
  %addtmp = add i32 %j3, 1
  %11 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  store i32 %addtmp, i32* %11
  %12 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  %j4 = load i32, i32* %12
  %greaterorequaltmp = icmp sge i32 %j4, 3
  br i1 %greaterorequaltmp, label %then, label %endif

after_loop2:                                      ; preds = %then
  %13 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %i5 = load i32, i32* %13
  %addtmp6 = add i32 %i5, 1
  %14 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  store i32 %addtmp6, i32* %14
  %15 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %i7 = load i32, i32* %15
  %greaterorequaltmp8 = icmp sge i32 %i7, 3
  br i1 %greaterorequaltmp8, label %then9, label %endif10

then:                                             ; preds = %loop1
  br label %after_loop2

endif:                                            ; preds = %break, %loop1
  br label %loop1

break:                                            ; No predecessors!
  br label %endif

then9:                                            ; preds = %after_loop2
  br label %after_loop

endif10:                                          ; preds = %break11, %after_loop2
  br label %loop

break11:                                          ; No predecessors!
  br label %endif10

loop12:                                           ; preds = %endif30, %after_loop
  %16 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  store i32 0, i32* %16
  br label %loop14

after_loop13:                                     ; preds = %then29
  %17 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 1
  %18 = getelementptr inbounds [3 x [3 x i32]], [3 x [3 x i32]]* %17, i32 0, i32 0
  %isSymmetric = call i32 @isSymmetric(%struct_main* %new_frame, [3 x i32]* %18)
  br i32 %isSymmetric, label %then32, label %else

loop14:                                           ; preds = %endif23, %loop12
  %19 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  %j16 = load i32, i32* %19
  %20 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %i17 = load i32, i32* %20
  %21 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 1
  %22 = getelementptr inbounds [3 x [3 x i32]], [3 x [3 x i32]]* %21, i32 0, i32 %i17
  %23 = getelementptr inbounds [3 x i32], [3 x i32]* %22, i32 0, i32 %j16
  %24 = load i32, i32* %23
  call void @writeInteger(i32 %24)
  call void @writeChar(i8 32)
  %25 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  %j18 = load i32, i32* %25
  %addtmp19 = add i32 %j18, 1
  %26 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  store i32 %addtmp19, i32* %26
  %27 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  %j20 = load i32, i32* %27
  %greaterorequaltmp21 = icmp sge i32 %j20, 3
  br i1 %greaterorequaltmp21, label %then22, label %endif23

after_loop15:                                     ; preds = %then22
  call void @writeChar(i8 10)
  %28 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %i25 = load i32, i32* %28
  %addtmp26 = add i32 %i25, 1
  %29 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  store i32 %addtmp26, i32* %29
  %30 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %i27 = load i32, i32* %30
  %greaterorequaltmp28 = icmp sge i32 %i27, 3
  br i1 %greaterorequaltmp28, label %then29, label %endif30

then22:                                           ; preds = %loop14
  br label %after_loop15

endif23:                                          ; preds = %break24, %loop14
  br label %loop14

break24:                                          ; No predecessors!
  br label %endif23

then29:                                           ; preds = %after_loop15
  br label %after_loop13

endif30:                                          ; preds = %break31, %after_loop15
  br label %loop12

break31:                                          ; No predecessors!
  br label %endif30

then32:                                           ; preds = %after_loop13
  call void @writeString(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @1, i32 0, i32 0))
  br label %endif33

endif33:                                          ; preds = %else, %then32
  %31 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 1
  %32 = getelementptr inbounds [3 x [3 x i32]], [3 x [3 x i32]]* %31, i32 0, i32 0
  %isTriangular = call i32 @isTriangular(%struct_main* %new_frame, [3 x i32]* %32)
  %33 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  store i32 %isTriangular, i32* %33
  %34 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %i34 = load i32, i32* %34
  %equaltmp = icmp eq i32 %i34, 0
  br i1 %equaltmp, label %then35, label %elif

else:                                             ; preds = %after_loop13
  call void @writeString(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @2, i32 0, i32 0))
  br label %endif33

then35:                                           ; preds = %endif33
  call void @writeString(i8* getelementptr inbounds ([30 x i8], [30 x i8]* @3, i32 0, i32 0))
  br label %endif36

endif36:                                          ; preds = %in_elif47, %next_elif43, %in_elif42, %in_elif, %then35
  br label %return

elif:                                             ; preds = %endif33
  %35 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %i37 = load i32, i32* %35
  %equaltmp38 = icmp eq i32 %i37, 1
  %elif_cond = icmp ne i1 %equaltmp38, i32 0
  br i1 %elif_cond, label %in_elif, label %next_elif

in_elif:                                          ; preds = %elif
  call void @writeString(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @4, i32 0, i32 0))
  br label %endif36

next_elif:                                        ; preds = %elif
  %36 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %i39 = load i32, i32* %36
  %equaltmp40 = icmp eq i32 %i39, 2
  %elif_cond41 = icmp ne i1 %equaltmp40, i32 0
  br i1 %elif_cond41, label %in_elif42, label %next_elif43

in_elif42:                                        ; preds = %next_elif
  call void @writeString(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @5, i32 0, i32 0))
  br label %endif36

next_elif43:                                      ; preds = %next_elif
  %37 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %i44 = load i32, i32* %37
  %equaltmp45 = icmp eq i32 %i44, 3
  %elif_cond46 = icmp ne i1 %equaltmp45, i32 0
  br i1 %elif_cond46, label %in_elif47, label %endif36

in_elif47:                                        ; preds = %next_elif43
  call void @writeString(i8* getelementptr inbounds ([53 x i8], [53 x i8]* @6, i32 0, i32 0))
  br label %endif36
}

define i32 @isSymmetric(%struct_main* %previous, [3 x [3 x i32]]* %a) {
entry:
  %new_frame = alloca %struct_isSymmetric
  %0 = alloca i32
  %1 = getelementptr %struct_isSymmetric, %struct_isSymmetric* %new_frame, i32 0, i32 0
  store %struct_main* %previous, %struct_main** %1
  %2 = getelementptr %struct_isSymmetric, %struct_isSymmetric* %new_frame, i32 0, i32 1
  store [3 x [3 x i32]]* %a, [3 x [3 x i32]]** %2
  %3 = getelementptr %struct_isSymmetric, %struct_isSymmetric* %new_frame, i32 0, i32 2
  store i32 0, i32* %3
  br label %loop

return:                                           ; preds = %after_return18, %after_loop, %then
  %4 = load i32, i32* %0
  ret i32 %4

loop:                                             ; preds = %endif16, %entry
  %5 = getelementptr %struct_isSymmetric, %struct_isSymmetric* %new_frame, i32 0, i32 2
  %i = load i32, i32* %5
  %addtmp = add i32 %i, 1
  %6 = getelementptr %struct_isSymmetric, %struct_isSymmetric* %new_frame, i32 0, i32 3
  store i32 %addtmp, i32* %6
  br label %loop1

after_loop:                                       ; preds = %then15
  store i32 1, i32* %0
  br label %return

loop1:                                            ; preds = %endif10, %loop
  %7 = getelementptr %struct_isSymmetric, %struct_isSymmetric* %new_frame, i32 0, i32 3
  %j = load i32, i32* %7
  %8 = getelementptr %struct_isSymmetric, %struct_isSymmetric* %new_frame, i32 0, i32 2
  %i3 = load i32, i32* %8
  %9 = getelementptr %struct_isSymmetric, %struct_isSymmetric* %new_frame, i32 0, i32 1
  %10 = load [3 x [3 x i32]]*, [3 x [3 x i32]]** %9
  %11 = getelementptr inbounds [3 x i32], [3 x [3 x i32]]* %10, i32 %i3
  %12 = load [3 x i32], [3 x i32]* %11
  %13 = load [3 x i32], [3 x i32]* %11
  %14 = getelementptr inbounds i32, [3 x i32] %13, i32 %j
  %15 = load i32, i32* %14
  %16 = getelementptr %struct_isSymmetric, %struct_isSymmetric* %new_frame, i32 0, i32 2
  %i4 = load i32, i32* %16
  %17 = getelementptr %struct_isSymmetric, %struct_isSymmetric* %new_frame, i32 0, i32 3
  %j5 = load i32, i32* %17
  %18 = getelementptr %struct_isSymmetric, %struct_isSymmetric* %new_frame, i32 0, i32 1
  %19 = load [3 x [3 x i32]]*, [3 x [3 x i32]]** %18
  %20 = getelementptr inbounds [3 x i32], [3 x [3 x i32]]* %19, i32 %j5
  %21 = load [3 x i32], [3 x i32]* %20
  %22 = load [3 x i32], [3 x i32]* %20
  %23 = getelementptr inbounds i32, [3 x i32] %22, i32 %i4
  %24 = load i32, i32* %23
  %nequaltmp = icmp ne i32 %15, %24
  br i1 %nequaltmp, label %then, label %endif

after_loop2:                                      ; preds = %then9
  %25 = getelementptr %struct_isSymmetric, %struct_isSymmetric* %new_frame, i32 0, i32 2
  %i11 = load i32, i32* %25
  %addtmp12 = add i32 %i11, 1
  %26 = getelementptr %struct_isSymmetric, %struct_isSymmetric* %new_frame, i32 0, i32 2
  store i32 %addtmp12, i32* %26
  %27 = getelementptr %struct_isSymmetric, %struct_isSymmetric* %new_frame, i32 0, i32 2
  %i13 = load i32, i32* %27
  %greaterorequaltmp14 = icmp sge i32 %i13, 2
  br i1 %greaterorequaltmp14, label %then15, label %endif16

then:                                             ; preds = %loop1
  store i32 0, i32* %0
  br label %return

endif:                                            ; preds = %after_return, %loop1
  %28 = getelementptr %struct_isSymmetric, %struct_isSymmetric* %new_frame, i32 0, i32 3
  %j6 = load i32, i32* %28
  %addtmp7 = add i32 %j6, 1
  %29 = getelementptr %struct_isSymmetric, %struct_isSymmetric* %new_frame, i32 0, i32 3
  store i32 %addtmp7, i32* %29
  %30 = getelementptr %struct_isSymmetric, %struct_isSymmetric* %new_frame, i32 0, i32 3
  %j8 = load i32, i32* %30
  %greaterorequaltmp = icmp sge i32 %j8, 3
  br i1 %greaterorequaltmp, label %then9, label %endif10

after_return:                                     ; No predecessors!
  br label %endif

then9:                                            ; preds = %endif
  br label %after_loop2

endif10:                                          ; preds = %break, %endif
  br label %loop1

break:                                            ; No predecessors!
  br label %endif10

then15:                                           ; preds = %after_loop2
  br label %after_loop

endif16:                                          ; preds = %break17, %after_loop2
  br label %loop

break17:                                          ; No predecessors!
  br label %endif16

after_return18:                                   ; No predecessors!
  br label %return
}

define i32 @isTriangular(%struct_main* %previous, [3 x [3 x i32]]* %a) {
entry:
  %new_frame = alloca %struct_isTriangular
  %0 = alloca i32
  %1 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 0
  store %struct_main* %previous, %struct_main** %1
  %2 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 1
  store [3 x [3 x i32]]* %a, [3 x [3 x i32]]** %2
  %3 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 2
  store i32 1, i32* %3
  %4 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 4
  store i32 0, i32* %4
  br label %loop

return:                                           ; preds = %after_return, %after_loop18
  %5 = load i32, i32* %0
  ret i32 %5

loop:                                             ; preds = %endif15, %entry
  %6 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 4
  %i = load i32, i32* %6
  %addtmp = add i32 %i, 1
  %7 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 5
  store i32 %addtmp, i32* %7
  br label %loop1

after_loop:                                       ; preds = %then14
  %8 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 3
  store i32 2, i32* %8
  %9 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 4
  store i32 1, i32* %9
  br label %loop17

loop1:                                            ; preds = %endif8, %loop
  %10 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 5
  %j = load i32, i32* %10
  %11 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 4
  %i3 = load i32, i32* %11
  %12 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 1
  %13 = load [3 x [3 x i32]]*, [3 x [3 x i32]]** %12
  %14 = getelementptr inbounds [3 x i32], [3 x [3 x i32]]* %13, i32 %i3
  %15 = load [3 x i32], [3 x i32]* %14
  %16 = load [3 x i32], [3 x i32]* %14
  %17 = getelementptr inbounds i32, [3 x i32] %16, i32 %j
  %18 = load i32, i32* %17
  %nequaltmp = icmp ne i32 %18, 0
  br i1 %nequaltmp, label %then, label %endif

after_loop2:                                      ; preds = %then7, %then
  %19 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 4
  %i10 = load i32, i32* %19
  %addtmp11 = add i32 %i10, 1
  %20 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 4
  store i32 %addtmp11, i32* %20
  %21 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 4
  %i12 = load i32, i32* %21
  %greaterorequaltmp13 = icmp sge i32 %i12, 2
  br i1 %greaterorequaltmp13, label %then14, label %endif15

then:                                             ; preds = %loop1
  %22 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 2
  store i32 0, i32* %22
  br label %after_loop2

endif:                                            ; preds = %break, %loop1
  %23 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 5
  %j4 = load i32, i32* %23
  %addtmp5 = add i32 %j4, 1
  %24 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 5
  store i32 %addtmp5, i32* %24
  %25 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 5
  %j6 = load i32, i32* %25
  %greaterorequaltmp = icmp sge i32 %j6, 3
  br i1 %greaterorequaltmp, label %then7, label %endif8

break:                                            ; No predecessors!
  br label %endif

then7:                                            ; preds = %endif
  br label %after_loop2

endif8:                                           ; preds = %break9, %endif
  br label %loop1

break9:                                           ; No predecessors!
  br label %endif8

then14:                                           ; preds = %after_loop2
  br label %after_loop

endif15:                                          ; preds = %break16, %after_loop2
  br label %loop

break16:                                          ; No predecessors!
  br label %endif15

loop17:                                           ; preds = %endif40, %after_loop
  %26 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 5
  store i32 0, i32* %26
  br label %loop19

after_loop18:                                     ; preds = %then39
  %27 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 2
  %lower = load i32, i32* %27
  %28 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 3
  %upper = load i32, i32* %28
  %addtmp42 = add i32 %lower, %upper
  store i32 %addtmp42, i32* %0
  br label %return

loop19:                                           ; preds = %endif33, %loop17
  %29 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 5
  %j21 = load i32, i32* %29
  %30 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 4
  %i22 = load i32, i32* %30
  %31 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 1
  %32 = load [3 x [3 x i32]]*, [3 x [3 x i32]]** %31
  %33 = getelementptr inbounds [3 x i32], [3 x [3 x i32]]* %32, i32 %i22
  %34 = load [3 x i32], [3 x i32]* %33
  %35 = load [3 x i32], [3 x i32]* %33
  %36 = getelementptr inbounds i32, [3 x i32] %35, i32 %j21
  %37 = load i32, i32* %36
  %nequaltmp23 = icmp ne i32 %37, 0
  br i1 %nequaltmp23, label %then24, label %endif25

after_loop20:                                     ; preds = %then32, %then24
  %38 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 4
  %i35 = load i32, i32* %38
  %addtmp36 = add i32 %i35, 1
  %39 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 4
  store i32 %addtmp36, i32* %39
  %40 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 4
  %i37 = load i32, i32* %40
  %greaterorequaltmp38 = icmp sge i32 %i37, 3
  br i1 %greaterorequaltmp38, label %then39, label %endif40

then24:                                           ; preds = %loop19
  %41 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 3
  store i32 0, i32* %41
  br label %after_loop20

endif25:                                          ; preds = %break26, %loop19
  %42 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 5
  %j27 = load i32, i32* %42
  %addtmp28 = add i32 %j27, 1
  %43 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 5
  store i32 %addtmp28, i32* %43
  %44 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 5
  %j29 = load i32, i32* %44
  %45 = getelementptr %struct_isTriangular, %struct_isTriangular* %new_frame, i32 0, i32 4
  %i30 = load i32, i32* %45
  %greaterorequaltmp31 = icmp sge i32 %j29, %i30
  br i1 %greaterorequaltmp31, label %then32, label %endif33

break26:                                          ; No predecessors!
  br label %endif25

then32:                                           ; preds = %endif25
  br label %after_loop20

endif33:                                          ; preds = %break34, %endif25
  br label %loop19

break34:                                          ; No predecessors!
  br label %endif33

then39:                                           ; preds = %after_loop20
  br label %after_loop18

endif40:                                          ; preds = %break41, %after_loop20
  br label %loop17

break41:                                          ; No predecessors!
  br label %endif40

after_return:                                     ; No predecessors!
  br label %return
}
