#include <decoder.h>
#include <glog/logging.h>
#include <../reader.h>
#include <../transforms.h>

Image Decode(std::istream& input) {
    BitReader bit_reader(input);
    JpegReader jpeg_reader(bit_reader);
    jpeg_reader.Read();
    Stats& stats = jpeg_reader.GetStats();
    std::vector<std::vector<double>> blocks;
    jpeg_reader.Scan(blocks);

    jpeg_reader.CheckEnd();

    Convert(blocks);

    DCTTransform(blocks);

    Image image(stats.width_, stats.height_);
    image.SetComment(stats.comment_);
    ConstructImage(blocks, image, stats);

    return image;
}
