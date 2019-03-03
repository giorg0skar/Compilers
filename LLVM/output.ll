; ModuleID = 'dana program'
source_filename = "dana program"

%dummy = type opaque
%struct_main = type { %dummy*, i32, i32, [24 x i32] }
%struct_quicksort = type { %struct_main*, i32*, i32, i32, i32, i32, i32, i32 }
%struct_writeArray = type { %struct_main*, i32, i32*, i32 }

@0 = private constant [3 x i8] c", \00"
@1 = private constant [2 x i8] c"\0A\00"

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
  store i32 65, i32* %1
  %2 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  store i32 0, i32* %2
  br label %loop

return:                                           ; preds = %after_loop
  ret void

loop:                                             ; preds = %endif, %entry
  %3 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %i = load i32, i32* %3
  %lowertmp = icmp slt i32 %i, 16
  br i1 %lowertmp, label %then, label %else

after_loop:                                       ; preds = %else
  %4 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  %5 = getelementptr inbounds [24 x i32], [24 x i32]* %4, i32 0, i32 0
  call void @writeArray(%struct_main* %new_frame, i32 16, i32* %5)
  %6 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  %7 = getelementptr inbounds [24 x i32], [24 x i32]* %6, i32 0, i32 0
  call void @quicksort(%struct_main* %new_frame, i32* %7, i32 0, i32 15)
  %8 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  %9 = getelementptr inbounds [24 x i32], [24 x i32]* %8, i32 0, i32 0
  call void @writeArray(%struct_main* %new_frame, i32 16, i32* %9)
  br label %return

then:                                             ; preds = %loop
  %10 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 1
  %seed = load i32, i32* %10
  %multmp = mul i32 %seed, 137
  %addtmp = add i32 %multmp, 220
  %11 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %i1 = load i32, i32* %11
  %addtmp2 = add i32 %addtmp, %i1
  %modtmp = srem i32 %addtmp2, 145
  %12 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 1
  store i32 %modtmp, i32* %12
  %13 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 1
  %seed3 = load i32, i32* %13
  %14 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %i4 = load i32, i32* %14
  %15 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 3
  %16 = getelementptr inbounds [24 x i32], [24 x i32]* %15, i32 0, i32 %i4
  store i32 %seed3, i32* %16
  %17 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  %i5 = load i32, i32* %17
  %addtmp6 = add i32 %i5, 1
  %18 = getelementptr %struct_main, %struct_main* %new_frame, i32 0, i32 2
  store i32 %addtmp6, i32* %18
  br label %endif

endif:                                            ; preds = %break, %then
  br label %loop

else:                                             ; preds = %loop
  br label %after_loop

break:                                            ; No predecessors!
  br label %endif
}

define void @quicksort(%struct_main* %previous, i32* %arr, i32 %low, i32 %high) {
entry:
  %new_frame = alloca %struct_quicksort
  %0 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 0
  store %struct_main* %previous, %struct_main** %0
  %1 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 1
  store i32* %arr, i32** %1
  %2 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 2
  store i32 %low, i32* %2
  %3 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 3
  store i32 %high, i32* %3
  %4 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 2
  %low1 = load i32, i32* %4
  %5 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 3
  %high2 = load i32, i32* %5
  %lowertmp = icmp slt i32 %low1, %high2
  br i1 %lowertmp, label %then, label %endif

return:                                           ; preds = %endif
  ret void

then:                                             ; preds = %entry
  %6 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 2
  %low3 = load i32, i32* %6
  %7 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 4
  store i32 %low3, i32* %7
  %8 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 2
  %low4 = load i32, i32* %8
  %9 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 5
  store i32 %low4, i32* %9
  %10 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 3
  %high5 = load i32, i32* %10
  %11 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 6
  store i32 %high5, i32* %11
  br label %loop

endif:                                            ; preds = %after_loop, %entry
  br label %return

loop:                                             ; preds = %endif37, %then
  %12 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 5
  %i = load i32, i32* %12
  %13 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 6
  %j = load i32, i32* %13
  %greatertmp = icmp sgt i32 %i, %j
  br i1 %greatertmp, label %then6, label %endif7

after_loop:                                       ; preds = %then6
  %14 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 6
  %j42 = load i32, i32* %14
  %15 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 1
  %16 = load i32*, i32** %15
  %17 = getelementptr inbounds i32, i32* %16, i32 %j42
  %18 = load i32, i32* %17
  %19 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 7
  store i32 %18, i32* %19
  %20 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 4
  %pivot43 = load i32, i32* %20
  %21 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 1
  %22 = load i32*, i32** %21
  %23 = getelementptr inbounds i32, i32* %22, i32 %pivot43
  %24 = load i32, i32* %23
  %25 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 6
  %j44 = load i32, i32* %25
  %26 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 1
  %27 = load i32*, i32** %26
  %28 = getelementptr inbounds i32, i32* %27, i32 %j44
  store i32 %24, i32* %28
  %29 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 7
  %temp45 = load i32, i32* %29
  %30 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 4
  %pivot46 = load i32, i32* %30
  %31 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 1
  %32 = load i32*, i32** %31
  %33 = getelementptr inbounds i32, i32* %32, i32 %pivot46
  store i32 %temp45, i32* %33
  %34 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 0
  %previous47 = load %struct_main*, %struct_main** %34
  %35 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 1
  %arr48 = load i32*, i32** %35
  %36 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 2
  %low49 = load i32, i32* %36
  %37 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 6
  %j50 = load i32, i32* %37
  %subtmp51 = sub i32 %j50, 1
  call void @quicksort(%struct_main* %previous47, i32* %arr48, i32 %low49, i32 %subtmp51)
  %38 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 0
  %previous52 = load %struct_main*, %struct_main** %38
  %39 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 1
  %arr53 = load i32*, i32** %39
  %40 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 6
  %j54 = load i32, i32* %40
  %addtmp55 = add i32 %j54, 1
  %41 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 3
  %high56 = load i32, i32* %41
  call void @quicksort(%struct_main* %previous52, i32* %arr53, i32 %addtmp55, i32 %high56)
  br label %endif

then6:                                            ; preds = %loop
  br label %after_loop

endif7:                                           ; preds = %break, %loop
  br label %loop8

break:                                            ; No predecessors!
  br label %endif7

loop8:                                            ; preds = %endif15, %endif7
  %42 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 5
  %i10 = load i32, i32* %42
  %43 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 1
  %44 = load i32*, i32** %43
  %45 = getelementptr inbounds i32, i32* %44, i32 %i10
  %46 = load i32, i32* %45
  %47 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 4
  %pivot = load i32, i32* %47
  %48 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 1
  %49 = load i32*, i32** %48
  %50 = getelementptr inbounds i32, i32* %49, i32 %pivot
  %51 = load i32, i32* %50
  %lowerorequaltmp = icmp sle i32 %46, %51
  %52 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 5
  %i11 = load i32, i32* %52
  %53 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 3
  %high12 = load i32, i32* %53
  %lowerorequaltmp13 = icmp sle i32 %i11, %high12
  br i1 %lowerorequaltmp, label %right_cond_and, label %result_and

after_loop9:                                      ; preds = %then14
  br label %loop18

right_cond_and:                                   ; preds = %loop8
  br label %result_and

result_and:                                       ; preds = %right_cond_and, %loop8
  %and = phi i8 [ 0, %loop8 ], [ %lowerorequaltmp13, %right_cond_and ]
  %cond_nottmp = icmp eq i8 %and, 0
  br i1 %cond_nottmp, label %then14, label %endif15

then14:                                           ; preds = %result_and
  br label %after_loop9

endif15:                                          ; preds = %break16, %result_and
  %54 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 5
  %i17 = load i32, i32* %54
  %addtmp = add i32 %i17, 1
  %55 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 5
  store i32 %addtmp, i32* %55
  br label %loop8

break16:                                          ; No predecessors!
  br label %endif15

loop18:                                           ; preds = %endif30, %after_loop9
  %56 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 6
  %j20 = load i32, i32* %56
  %57 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 1
  %58 = load i32*, i32** %57
  %59 = getelementptr inbounds i32, i32* %58, i32 %j20
  %60 = load i32, i32* %59
  %61 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 4
  %pivot21 = load i32, i32* %61
  %62 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 1
  %63 = load i32*, i32** %62
  %64 = getelementptr inbounds i32, i32* %63, i32 %pivot21
  %65 = load i32, i32* %64
  %greatertmp22 = icmp sgt i32 %60, %65
  %66 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 6
  %j23 = load i32, i32* %66
  %67 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 2
  %low24 = load i32, i32* %67
  %greaterorequaltmp = icmp sge i32 %j23, %low24
  br i1 %greatertmp22, label %right_cond_and25, label %result_and26

after_loop19:                                     ; preds = %then29
  %68 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 5
  %i33 = load i32, i32* %68
  %69 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 6
  %j34 = load i32, i32* %69
  %lowertmp35 = icmp slt i32 %i33, %j34
  br i1 %lowertmp35, label %then36, label %endif37

right_cond_and25:                                 ; preds = %loop18
  br label %result_and26

result_and26:                                     ; preds = %right_cond_and25, %loop18
  %and27 = phi i8 [ 0, %loop18 ], [ %greaterorequaltmp, %right_cond_and25 ]
  %cond_nottmp28 = icmp eq i8 %and27, 0
  br i1 %cond_nottmp28, label %then29, label %endif30

then29:                                           ; preds = %result_and26
  br label %after_loop19

endif30:                                          ; preds = %break31, %result_and26
  %70 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 6
  %j32 = load i32, i32* %70
  %subtmp = sub i32 %j32, 1
  %71 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 6
  store i32 %subtmp, i32* %71
  br label %loop18

break31:                                          ; No predecessors!
  br label %endif30

then36:                                           ; preds = %after_loop19
  %72 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 5
  %i38 = load i32, i32* %72
  %73 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 1
  %74 = load i32*, i32** %73
  %75 = getelementptr inbounds i32, i32* %74, i32 %i38
  %76 = load i32, i32* %75
  %77 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 7
  store i32 %76, i32* %77
  %78 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 6
  %j39 = load i32, i32* %78
  %79 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 1
  %80 = load i32*, i32** %79
  %81 = getelementptr inbounds i32, i32* %80, i32 %j39
  %82 = load i32, i32* %81
  %83 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 5
  %i40 = load i32, i32* %83
  %84 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 1
  %85 = load i32*, i32** %84
  %86 = getelementptr inbounds i32, i32* %85, i32 %i40
  store i32 %82, i32* %86
  %87 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 7
  %temp = load i32, i32* %87
  %88 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 6
  %j41 = load i32, i32* %88
  %89 = getelementptr %struct_quicksort, %struct_quicksort* %new_frame, i32 0, i32 1
  %90 = load i32*, i32** %89
  %91 = getelementptr inbounds i32, i32* %90, i32 %j41
  store i32 %temp, i32* %91
  br label %endif37

endif37:                                          ; preds = %then36, %after_loop19
  br label %loop
}

define void @writeArray(%struct_main* %previous, i32 %n, i32* %x) {
entry:
  %new_frame = alloca %struct_writeArray
  %0 = getelementptr %struct_writeArray, %struct_writeArray* %new_frame, i32 0, i32 0
  store %struct_main* %previous, %struct_main** %0
  %1 = getelementptr %struct_writeArray, %struct_writeArray* %new_frame, i32 0, i32 1
  store i32 %n, i32* %1
  %2 = getelementptr %struct_writeArray, %struct_writeArray* %new_frame, i32 0, i32 2
  store i32* %x, i32** %2
  %3 = getelementptr %struct_writeArray, %struct_writeArray* %new_frame, i32 0, i32 3
  store i32 0, i32* %3
  br label %loop

return:                                           ; preds = %after_loop
  ret void

loop:                                             ; preds = %endif, %entry
  %4 = getelementptr %struct_writeArray, %struct_writeArray* %new_frame, i32 0, i32 3
  %i = load i32, i32* %4
  %5 = getelementptr %struct_writeArray, %struct_writeArray* %new_frame, i32 0, i32 1
  %n1 = load i32, i32* %5
  %lowertmp = icmp slt i32 %i, %n1
  br i1 %lowertmp, label %then, label %else

after_loop:                                       ; preds = %else
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @1, i32 0, i32 0))
  br label %return

then:                                             ; preds = %loop
  %6 = getelementptr %struct_writeArray, %struct_writeArray* %new_frame, i32 0, i32 3
  %i2 = load i32, i32* %6
  %greatertmp = icmp sgt i32 %i2, 0
  br i1 %greatertmp, label %then3, label %endif4

endif:                                            ; preds = %break, %endif4
  br label %loop

else:                                             ; preds = %loop
  br label %after_loop

then3:                                            ; preds = %then
  call void @writeString(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i32 0, i32 0))
  br label %endif4

endif4:                                           ; preds = %then3, %then
  %7 = getelementptr %struct_writeArray, %struct_writeArray* %new_frame, i32 0, i32 3
  %i5 = load i32, i32* %7
  %8 = getelementptr %struct_writeArray, %struct_writeArray* %new_frame, i32 0, i32 2
  %9 = load i32*, i32** %8
  %10 = getelementptr inbounds i32, i32* %9, i32 %i5
  %11 = load i32, i32* %10
  call void @writeInteger(i32 %11)
  %12 = getelementptr %struct_writeArray, %struct_writeArray* %new_frame, i32 0, i32 3
  %i6 = load i32, i32* %12
  %addtmp = add i32 %i6, 1
  %13 = getelementptr %struct_writeArray, %struct_writeArray* %new_frame, i32 0, i32 3
  store i32 %addtmp, i32* %13
  br label %endif

break:                                            ; No predecessors!
  br label %endif
}
