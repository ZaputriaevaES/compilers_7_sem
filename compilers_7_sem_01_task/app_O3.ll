; ModuleID = 'app.c'
source_filename = "app.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define dso_local void @app() local_unnamed_addr #0 {
  br label %1

1:                                                ; preds = %0, %40
  %2 = phi i32 [ 0, %0 ], [ %41, %40 ]
  %3 = phi i32 [ 13458524, %0 ], [ %25, %40 ]
  %4 = phi i32 [ 226, %0 ], [ %26, %40 ]
  %5 = phi i32 [ 5, %0 ], [ %24, %40 ]
  %6 = phi i32 [ 4, %0 ], [ %17, %40 ]
  %7 = phi i32 [ 108, %0 ], [ %27, %40 ]
  br label %9

8:                                                ; preds = %40
  ret void

9:                                                ; preds = %1, %30
  %10 = phi i32 [ 0, %1 ], [ %31, %30 ]
  br label %33

11:                                               ; preds = %30
  %12 = add i32 %4, -451
  %13 = icmp ult i32 %12, -450
  %14 = sub nsw i32 0, %6
  %15 = icmp eq i32 %3, 13458524
  %16 = select i1 %15, i32 9498256, i32 13458524
  %17 = select i1 %13, i32 %14, i32 %6
  %18 = select i1 %13, i32 %16, i32 %3
  %19 = add i32 %7, -215
  %20 = icmp ult i32 %19, -214
  %21 = sub nsw i32 0, %5
  %22 = icmp eq i32 %18, 13458524
  %23 = select i1 %22, i32 9498256, i32 13458524
  %24 = select i1 %20, i32 %21, i32 %5
  %25 = select i1 %20, i32 %23, i32 %18
  %26 = add nsw i32 %17, %4
  %27 = add nsw i32 %24, %7
  %28 = add nsw i32 %26, 59
  %29 = add nsw i32 %27, 39
  br label %37

30:                                               ; preds = %33
  %31 = add nuw nsw i32 %10, 1
  %32 = icmp eq i32 %31, 256
  br i1 %32, label %11, label %9, !llvm.loop !5

33:                                               ; preds = %9, %33
  %34 = phi i32 [ 0, %9 ], [ %35, %33 ]
  tail call void @simPutPixel(i32 noundef %34, i32 noundef %10, i32 noundef -16777216) #2
  %35 = add nuw nsw i32 %34, 1
  %36 = icmp eq i32 %35, 512
  br i1 %36, label %30, label %33, !llvm.loop !7

37:                                               ; preds = %11, %43
  %38 = phi i32 [ %27, %11 ], [ %44, %43 ]
  %39 = icmp ult i32 %38, 256
  br i1 %39, label %46, label %43

40:                                               ; preds = %43
  tail call void (...) @simFlush() #2
  %41 = add nuw nsw i32 %2, 1
  %42 = icmp eq i32 %41, 200
  br i1 %42, label %8, label %1, !llvm.loop !8

43:                                               ; preds = %50, %37
  %44 = add nsw i32 %38, 1
  %45 = icmp slt i32 %38, %29
  br i1 %45, label %37, label %40, !llvm.loop !9

46:                                               ; preds = %37, %50
  %47 = phi i32 [ %51, %50 ], [ %26, %37 ]
  %48 = icmp ult i32 %47, 512
  br i1 %48, label %49, label %50

49:                                               ; preds = %46
  tail call void @simPutPixel(i32 noundef %47, i32 noundef %38, i32 noundef %25) #2
  br label %50

50:                                               ; preds = %46, %49
  %51 = add nsw i32 %47, 1
  %52 = icmp slt i32 %47, %28
  br i1 %52, label %46, label %43, !llvm.loop !10
}

declare void @simPutPixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #1

declare void @simFlush(...) local_unnamed_addr #1

attributes #0 = { nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!5 = distinct !{!5, !6}
!6 = !{!"llvm.loop.mustprogress"}
!7 = distinct !{!7, !6}
!8 = distinct !{!8, !6}
!9 = distinct !{!9, !6}
!10 = distinct !{!10, !6}
