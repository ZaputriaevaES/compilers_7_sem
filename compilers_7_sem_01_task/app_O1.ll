; ModuleID = 'app.c'
source_filename = "app.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define dso_local void @app() local_unnamed_addr #0 {
  br label %1

1:                                                ; preds = %0, %41
  %2 = phi i32 [ 0, %0 ], [ %42, %41 ]
  %3 = phi i32 [ 13458524, %0 ], [ %25, %41 ]
  %4 = phi i32 [ 226, %0 ], [ %26, %41 ]
  %5 = phi i32 [ 5, %0 ], [ %24, %41 ]
  %6 = phi i32 [ 4, %0 ], [ %17, %41 ]
  %7 = phi i32 [ 108, %0 ], [ %27, %41 ]
  br label %9

8:                                                ; preds = %41
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
  call void @simPutPixel(i32 noundef %34, i32 noundef %10, i32 noundef -16777216) #2
  %35 = add nuw nsw i32 %34, 1
  %36 = icmp eq i32 %35, 512
  br i1 %36, label %30, label %33, !llvm.loop !8

37:                                               ; preds = %11, %44
  %38 = phi i32 [ %27, %11 ], [ %45, %44 ]
  %39 = icmp sgt i32 %38, -1
  %40 = icmp slt i32 %38, 256
  br label %47

41:                                               ; preds = %44
  call void (...) @simFlush() #2
  %42 = add nuw nsw i32 %2, 1
  %43 = icmp eq i32 %42, 200
  br i1 %43, label %8, label %1, !llvm.loop !9

44:                                               ; preds = %53
  %45 = add nsw i32 %38, 1
  %46 = icmp slt i32 %38, %29
  br i1 %46, label %37, label %41, !llvm.loop !10

47:                                               ; preds = %37, %53
  %48 = phi i32 [ %26, %37 ], [ %54, %53 ]
  %49 = icmp ult i32 %48, 512
  %50 = select i1 %49, i1 %39, i1 false
  %51 = select i1 %50, i1 %40, i1 false
  br i1 %51, label %52, label %53

52:                                               ; preds = %47
  call void @simPutPixel(i32 noundef %48, i32 noundef %38, i32 noundef %25) #2
  br label %53

53:                                               ; preds = %47, %52
  %54 = add nsw i32 %48, 1
  %55 = icmp slt i32 %48, %28
  br i1 %55, label %47, label %44, !llvm.loop !11
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
!5 = distinct !{!5, !6, !7}
!6 = !{!"llvm.loop.mustprogress"}
!7 = !{!"llvm.loop.unroll.disable"}
!8 = distinct !{!8, !6, !7}
!9 = distinct !{!9, !6, !7}
!10 = distinct !{!10, !6, !7}
!11 = distinct !{!11, !6, !7}
