; ModuleID = 'dana program'
source_filename = "dana program"

%dummy = type opaque
%struct_main = type { %dummy*, [15 x i32], [16 x i32], [16 x i32], i32, i32, i32, i32, i8 }
%struct_initializeSet = type { %struct_main*, i32* }
%struct_addElement = type { %struct_main*, i32*, i32, i32 }
%struct_printSet = type { %struct_main*, i32*, i32 }
%struct_log3binary = type { %struct_main*, i32, i32, i32, i32, i32 }
%struct_getSum = type { %struct_main*, i32 }

@0 = private constant [2 x i8] c" \00"
@1 = private constant [7 x i8] c"[] []\0A\00"
@2 = private constant [2 x i8] c"\0A\00"

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
  %1 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 1
  %2 = getelementptr inbounds [15 x i32], [15 x i32]* %1, i32 0, i32 0
  store i32 1, i32* %2
  %3 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 4
  store i32 1, i32* %3
  br label %loop

return:                                           ; preds = %after_loop5, %then25
  ret void

loop:                                             ; preds = %endif, %entry
  %4 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 4
  %i = load i32, i32* %4
  %greaterorequaltmp = icmp sge i32 %i, 15
  br i1 %greaterorequaltmp, label %then, label %endif

after_loop:                                       ; preds = %then
  %5 = call i32 @readInteger()
  %6 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 6
  store i32 %5, i32* %6
  %7 = call i32 @readInteger()
  %8 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 7
  store i32 %7, i32* %8
  %9 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %10 = getelementptr inbounds [16 x i32], [16 x i32]* %9, i32 0, i32 0
  call void @initializeSet(%struct_main* %new_frame, i32* %10)
  %11 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  %12 = getelementptr inbounds [16 x i32], [16 x i32]* %11, i32 0, i32 0
  call void @initializeSet(%struct_main* %new_frame, i32* %12)
  %13 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 8
  store i8 1, i8* %13
  br label %loop4

then:                                             ; preds = %loop
  br label %after_loop

endif:                                            ; preds = %break, %loop
  %14 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 4
  %i1 = load i32, i32* %14
  %subtmp = sub i32 %i1, 1
  %15 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 1
  %16 = getelementptr inbounds [15 x i32], [15 x i32]* %15, i32 0, i32 %subtmp
  %17 = load i32, i32* %16
  %multmp = mul i32 %17, 3
  %18 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 4
  %i2 = load i32, i32* %18
  %19 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 1
  %20 = getelementptr inbounds [15 x i32], [15 x i32]* %19, i32 0, i32 %i2
  store i32 %multmp, i32* %20
  %21 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 4
  %i3 = load i32, i32* %21
  %addtmp = add i32 %i3, 1
  %22 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 4
  store i32 %addtmp, i32* %22
  br label %loop

break:                                            ; No predecessors!
  br label %endif

loop4:                                            ; preds = %endif12, %after_loop
  %23 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 7
  %M = load i32, i32* %23
  %equaltmp = icmp eq i32 %M, 0
  br i1 %equaltmp, label %then6, label %endif7

after_loop5:                                      ; preds = %then6
  %24 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  %25 = getelementptr inbounds [16 x i32], [16 x i32]* %24, i32 0, i32 0
  call void @printSet(%struct_main* %new_frame, i32* %25)
  call void @writeChar(i8 10)
  %26 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %27 = getelementptr inbounds [16 x i32], [16 x i32]* %26, i32 0, i32 0
  call void @printSet(%struct_main* %new_frame, i32* %27)
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @2, i32 0, i32 0))
  br label %return

then6:                                            ; preds = %loop4
  br label %after_loop5

endif7:                                           ; preds = %break8, %loop4
  %28 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 7
  %M9 = load i32, i32* %28
  %29 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 6
  %N = load i32, i32* %29
  %log3binary = call i32 @log3binary(%struct_main* %new_frame, i32 %M9, i32 %N)
  %30 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 5
  store i32 %log3binary, i32* %30
  %31 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 7
  %M10 = load i32, i32* %31
  %32 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 5
  %k = load i32, i32* %32
  %getSum = call i32 @getSum(%struct_main* %new_frame, i32 %k)
  %lowerorequaltmp = icmp sle i32 %M10, %getSum
  br i1 %lowerorequaltmp, label %then11, label %else

break8:                                           ; No predecessors!
  br label %endif7

then11:                                           ; preds = %endif7
  %33 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 7
  %M13 = load i32, i32* %33
  %34 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 5
  %k14 = load i32, i32* %34
  %35 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 1
  %36 = getelementptr inbounds [15 x i32], [15 x i32]* %35, i32 0, i32 %k14
  %37 = load i32, i32* %36
  %subtmp15 = sub i32 %M13, %37
  %38 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 7
  store i32 %subtmp15, i32* %38
  %39 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 8
  %flag = load i8, i8* %39
  br i8 %flag, label %then16, label %else18

endif12:                                          ; preds = %endif33, %endif17
  br label %loop4

else:                                             ; preds = %endif7
  %40 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 5
  %k21 = load i32, i32* %40
  %41 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 6
  %N22 = load i32, i32* %41
  %subtmp23 = sub i32 %N22, 1
  %equaltmp24 = icmp eq i32 %k21, %subtmp23
  br i1 %equaltmp24, label %then25, label %endif26

then16:                                           ; preds = %then11
  %42 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %43 = getelementptr inbounds [16 x i32], [16 x i32]* %42, i32 0, i32 0
  %44 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 5
  %k19 = load i32, i32* %44
  call void @addElement(%struct_main* %new_frame, i32* %43, i32 %k19)
  br label %endif17

endif17:                                          ; preds = %else18, %then16
  br label %endif12

else18:                                           ; preds = %then11
  %45 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  %46 = getelementptr inbounds [16 x i32], [16 x i32]* %45, i32 0, i32 0
  %47 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 5
  %k20 = load i32, i32* %47
  call void @addElement(%struct_main* %new_frame, i32* %46, i32 %k20)
  br label %endif17

then25:                                           ; preds = %else
  call void @writeString(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @1, i32 0, i32 0))
  br label %return

endif26:                                          ; preds = %exit, %else
  %48 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 5
  %k27 = load i32, i32* %48
  %addtmp28 = add i32 %k27, 1
  %49 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 1
  %50 = getelementptr inbounds [15 x i32], [15 x i32]* %49, i32 0, i32 %addtmp28
  %51 = load i32, i32* %50
  %52 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 7
  %M29 = load i32, i32* %52
  %subtmp30 = sub i32 %51, %M29
  %53 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 7
  store i32 %subtmp30, i32* %53
  %54 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 8
  %flag31 = load i8, i8* %54
  br i8 %flag31, label %then32, label %else34

exit:                                             ; No predecessors!
  br label %endif26

then32:                                           ; preds = %endif26
  %55 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %56 = getelementptr inbounds [16 x i32], [16 x i32]* %55, i32 0, i32 0
  %57 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 5
  %k35 = load i32, i32* %57
  %addtmp36 = add i32 %k35, 1
  call void @addElement(%struct_main* %new_frame, i32* %56, i32 %addtmp36)
  %58 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 8
  store i8 0, i8* %58
  br label %endif33

endif33:                                          ; preds = %else34, %then32
  br label %endif12

else34:                                           ; preds = %endif26
  %59 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  %60 = getelementptr inbounds [16 x i32], [16 x i32]* %59, i32 0, i32 0
  %61 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 5
  %k37 = load i32, i32* %61
  %addtmp38 = add i32 %k37, 1
  call void @addElement(%struct_main* %new_frame, i32* %60, i32 %addtmp38)
  %62 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 8
  store i8 1, i8* %62
  br label %endif33
}

define void @initializeSet(%struct_main* %previous, i32* %set) {
entry:
  %new_frame = alloca %struct_initializeSet
  %0 = getelementptr %struct_initializeSet, %struct_initializeSet* %new_frame, i32 0, i32 0
  store %struct_main* %previous, %struct_main** %0
  %1 = getelementptr %struct_initializeSet, %struct_initializeSet* %new_frame, i32 0, i32 1
  store i32* %set, i32** %1
  %2 = getelementptr %struct_initializeSet, %struct_initializeSet* %new_frame, i32 0, i32 1
  %3 = load i32*, i32** %2
  %4 = getelementptr inbounds i32, i32* %3, i32 0
  store i32 -1, i32* %4
  br label %return

return:                                           ; preds = %entry
  ret void
}

define void @addElement(%struct_main* %previous, i32* %set, i32 %n) {
entry:
  %new_frame = alloca %struct_addElement
  %0 = getelementptr %struct_addElement, %struct_addElement* %new_frame, i32 0, i32 0
  store %struct_main* %previous, %struct_main** %0
  %1 = getelementptr %struct_addElement, %struct_addElement* %new_frame, i32 0, i32 1
  store i32* %set, i32** %1
  %2 = getelementptr %struct_addElement, %struct_addElement* %new_frame, i32 0, i32 2
  store i32 %n, i32* %2
  %3 = getelementptr %struct_addElement, %struct_addElement* %new_frame, i32 0, i32 3
  store i32 0, i32* %3
  br label %loop

return:                                           ; preds = %after_loop
  ret void

loop:                                             ; preds = %endif, %entry
  %4 = getelementptr %struct_addElement, %struct_addElement* %new_frame, i32 0, i32 3
  %i = load i32, i32* %4
  %5 = getelementptr %struct_addElement, %struct_addElement* %new_frame, i32 0, i32 1
  %6 = load i32*, i32** %5
  %7 = getelementptr inbounds i32, i32* %6, i32 %i
  %8 = load i32, i32* %7
  %equaltmp = icmp eq i32 %8, -1
  br i1 %equaltmp, label %then, label %endif

after_loop:                                       ; preds = %then
  %9 = getelementptr %struct_addElement, %struct_addElement* %new_frame, i32 0, i32 2
  %n2 = load i32, i32* %9
  %10 = getelementptr %struct_addElement, %struct_addElement* %new_frame, i32 0, i32 3
  %i3 = load i32, i32* %10
  %11 = getelementptr %struct_addElement, %struct_addElement* %new_frame, i32 0, i32 1
  %12 = load i32*, i32** %11
  %13 = getelementptr inbounds i32, i32* %12, i32 %i3
  store i32 %n2, i32* %13
  %14 = getelementptr %struct_addElement, %struct_addElement* %new_frame, i32 0, i32 3
  %i4 = load i32, i32* %14
  %addtmp5 = add i32 %i4, 1
  %15 = getelementptr %struct_addElement, %struct_addElement* %new_frame, i32 0, i32 1
  %16 = load i32*, i32** %15
  %17 = getelementptr inbounds i32, i32* %16, i32 %addtmp5
  store i32 -1, i32* %17
  br label %return

then:                                             ; preds = %loop
  br label %after_loop

endif:                                            ; preds = %break, %loop
  %18 = getelementptr %struct_addElement, %struct_addElement* %new_frame, i32 0, i32 3
  %i1 = load i32, i32* %18
  %addtmp = add i32 %i1, 1
  %19 = getelementptr %struct_addElement, %struct_addElement* %new_frame, i32 0, i32 3
  store i32 %addtmp, i32* %19
  br label %loop

break:                                            ; No predecessors!
  br label %endif
}

define void @printSet(%struct_main* %previous, i32* %set) {
entry:
  %new_frame = alloca %struct_printSet
  %0 = getelementptr %struct_printSet, %struct_printSet* %new_frame, i32 0, i32 0
  store %struct_main* %previous, %struct_main** %0
  %1 = getelementptr %struct_printSet, %struct_printSet* %new_frame, i32 0, i32 1
  store i32* %set, i32** %1
  %2 = getelementptr %struct_printSet, %struct_printSet* %new_frame, i32 0, i32 2
  store i32 0, i32* %2
  br label %loop

return:                                           ; preds = %after_loop
  ret void

loop:                                             ; preds = %endif4, %entry
  %3 = getelementptr %struct_printSet, %struct_printSet* %new_frame, i32 0, i32 2
  %i = load i32, i32* %3
  %4 = getelementptr %struct_printSet, %struct_printSet* %new_frame, i32 0, i32 1
  %5 = load i32*, i32** %4
  %6 = getelementptr inbounds i32, i32* %5, i32 %i
  %7 = load i32, i32* %6
  %equaltmp = icmp eq i32 %7, -1
  br i1 %equaltmp, label %then, label %endif

after_loop:                                       ; preds = %then
  br label %return

then:                                             ; preds = %loop
  br label %after_loop

endif:                                            ; preds = %break, %loop
  %8 = getelementptr %struct_printSet, %struct_printSet* %new_frame, i32 0, i32 2
  %i1 = load i32, i32* %8
  %9 = getelementptr %struct_printSet, %struct_printSet* %new_frame, i32 0, i32 1
  %10 = load i32*, i32** %9
  %11 = getelementptr inbounds i32, i32* %10, i32 %i1
  %12 = load i32, i32* %11
  call void @writeInteger(i32 %12)
  %13 = getelementptr %struct_printSet, %struct_printSet* %new_frame, i32 0, i32 2
  %i2 = load i32, i32* %13
  %addtmp = add i32 %i2, 1
  %14 = getelementptr %struct_printSet, %struct_printSet* %new_frame, i32 0, i32 1
  %15 = load i32*, i32** %14
  %16 = getelementptr inbounds i32, i32* %15, i32 %addtmp
  %17 = load i32, i32* %16
  %nequaltmp = icmp ne i32 %17, -1
  br i1 %nequaltmp, label %then3, label %endif4

break:                                            ; No predecessors!
  br label %endif

then3:                                            ; preds = %endif
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @0, i32 0, i32 0))
  br label %endif4

endif4:                                           ; preds = %then3, %endif
  %18 = getelementptr %struct_printSet, %struct_printSet* %new_frame, i32 0, i32 2
  %i5 = load i32, i32* %18
  %addtmp6 = add i32 %i5, 1
  %19 = getelementptr %struct_printSet, %struct_printSet* %new_frame, i32 0, i32 2
  store i32 %addtmp6, i32* %19
  br label %loop
}

define i32 @log3binary(%struct_main* %previous, i32 %M, i32 %N) {
entry:
  %new_frame = alloca %struct_log3binary
  %0 = alloca i32
  %1 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 0
  store %struct_main* %previous, %struct_main** %1
  %2 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 1
  store i32 %M, i32* %2
  %3 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 2
  store i32 %N, i32* %3
  %4 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 3
  store i32 0, i32* %4
  %5 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 2
  %N1 = load i32, i32* %5
  %subtmp = sub i32 %N1, 1
  %6 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 4
  store i32 %subtmp, i32* %6
  br label %loop

return:                                           ; preds = %after_return27, %endif22, %then21, %then6
  %7 = load i32, i32* %0
  ret i32 %7

loop:                                             ; preds = %endif13, %entry
  %8 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 3
  %l = load i32, i32* %8
  %9 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 4
  %r = load i32, i32* %9
  %greaterorequaltmp = icmp sge i32 %l, %r
  br i1 %greaterorequaltmp, label %then, label %endif

after_loop:                                       ; preds = %then
  %10 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 3
  %l18 = load i32, i32* %10
  %11 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 0
  %previous19 = load %struct_main*, %struct_main** %11
  %12 = getelementptr %struct_main, %struct_main* %previous19, i32 0, i32 1
  %13 = getelementptr inbounds [15 x i32], [15 x i32]* %12, i32 0, i32 %l18
  %14 = load i32, i32* %13
  %15 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 1
  %M20 = load i32, i32* %15
  %lowerorequaltmp = icmp sle i32 %14, %M20
  br i1 %lowerorequaltmp, label %then21, label %endif22

then:                                             ; preds = %loop
  br label %after_loop

endif:                                            ; preds = %break, %loop
  %16 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 3
  %l2 = load i32, i32* %16
  %17 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 4
  %r3 = load i32, i32* %17
  %addtmp = add i32 %l2, %r3
  %divtmp = sdiv i32 %addtmp, 2
  %18 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 5
  store i32 %divtmp, i32* %18
  %19 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 5
  %mid = load i32, i32* %19
  %20 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 0
  %previous4 = load %struct_main*, %struct_main** %20
  %21 = getelementptr %struct_main, %struct_main* %previous4, i32 0, i32 1
  %22 = getelementptr inbounds [15 x i32], [15 x i32]* %21, i32 0, i32 %mid
  %23 = load i32, i32* %22
  %24 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 1
  %M5 = load i32, i32* %24
  %equaltmp = icmp eq i32 %23, %M5
  br i1 %equaltmp, label %then6, label %endif7

break:                                            ; No predecessors!
  br label %endif

then6:                                            ; preds = %endif
  %25 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 3
  %l8 = load i32, i32* %25
  store i32 %l8, i32* %0
  br label %return

endif7:                                           ; preds = %after_return, %endif
  %26 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 5
  %mid9 = load i32, i32* %26
  %27 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 0
  %previous10 = load %struct_main*, %struct_main** %27
  %28 = getelementptr %struct_main, %struct_main* %previous10, i32 0, i32 1
  %29 = getelementptr inbounds [15 x i32], [15 x i32]* %28, i32 0, i32 %mid9
  %30 = load i32, i32* %29
  %31 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 1
  %M11 = load i32, i32* %31
  %greatertmp = icmp sgt i32 %30, %M11
  br i1 %greatertmp, label %then12, label %else

after_return:                                     ; No predecessors!
  br label %endif7

then12:                                           ; preds = %endif7
  %32 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 5
  %mid14 = load i32, i32* %32
  %subtmp15 = sub i32 %mid14, 1
  %33 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 4
  store i32 %subtmp15, i32* %33
  br label %endif13

endif13:                                          ; preds = %else, %then12
  br label %loop

else:                                             ; preds = %endif7
  %34 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 5
  %mid16 = load i32, i32* %34
  %addtmp17 = add i32 %mid16, 1
  %35 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 3
  store i32 %addtmp17, i32* %35
  br label %endif13

then21:                                           ; preds = %after_loop
  %36 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 3
  %l23 = load i32, i32* %36
  store i32 %l23, i32* %0
  br label %return

endif22:                                          ; preds = %after_return24, %after_loop
  %37 = getelementptr %struct_log3binary, %struct_log3binary* %new_frame, i32 0, i32 3
  %l25 = load i32, i32* %37
  %subtmp26 = sub i32 %l25, 1
  store i32 %subtmp26, i32* %0
  br label %return

after_return24:                                   ; No predecessors!
  br label %endif22

after_return27:                                   ; No predecessors!
  br label %return
}

define i32 @getSum(%struct_main* %previous, i32 %k) {
entry:
  %new_frame = alloca %struct_getSum
  %0 = alloca i32
  %1 = getelementptr %struct_getSum, %struct_getSum* %new_frame, i32 0, i32 0
  store %struct_main* %previous, %struct_main** %1
  %2 = getelementptr %struct_getSum, %struct_getSum* %new_frame, i32 0, i32 1
  store i32 %k, i32* %2
  %3 = getelementptr %struct_getSum, %struct_getSum* %new_frame, i32 0, i32 1
  %k1 = load i32, i32* %3
  %addtmp = add i32 %k1, 1
  %4 = getelementptr %struct_getSum, %struct_getSum* %new_frame, i32 0, i32 0
  %previous2 = load %struct_main*, %struct_main** %4
  %5 = getelementptr %struct_main, %struct_main* %previous2, i32 0, i32 1
  %6 = getelementptr inbounds [15 x i32], [15 x i32]* %5, i32 0, i32 %addtmp
  %7 = load i32, i32* %6
  %subtmp = sub i32 %7, 1
  %divtmp = sdiv i32 %subtmp, 2
  store i32 %divtmp, i32* %0
  br label %return

return:                                           ; preds = %after_return, %entry
  %8 = load i32, i32* %0
  ret i32 %8

after_return:                                     ; No predecessors!
  br label %return
}
