# KeyframeGridCellMedianValues

Extract keyframes from video file,
record their origin timestamp,
convert frame into grayscale,
split each frame into a grid of given dimensions,
calculate median values of pixels of frame grid cells.


--------------------------------------------------------------------------------------------------------------------------------
Build dependencies

    g++
    libstdc++
    pthreads

    Boost - binary components: system filesystem program_options

    OpenCV - videoio core highgui imgproc ml features2d calib3d objdetect flann

    libav (ffmpeg, ffprobe) - avutil avcodec avformat avfilter swscale swresample
      Libraries, NOT simply ffmpeg and ffprobe executable shell commands.


Build commands

    ...KeyframeGridCellMedianValues/build$ cmake ..
    ...KeyframeGridCellMedianValues/build$ make -j4


Run command

    ./MainProject

    Default values are used in place of absent command-line arguments.

command-line arguments

    --help, -h

    --video, -v
      Video from which to extract keyframes.

    --csv, -c
      CSV file containing results of calculating median values of grayscale pixels of each grid cell for keyframe at reported video timestamp.

    --dim, -d
      Dimension of grid into which to split keyframe.
      Required to be an even divisor of the vertical and horizontal pixel counts of an extracted keyframe.

    example
      --video ../../../media/videoFileFromWhichToExtractKeyframes.mp4 --dim 4 --csv ../../../CSV_results/CSVTimestampAndGrayscaleKeyframeGridCellMedianValues.csv
