#include "mp3file.h"

using namespace reone::audio;

namespace reone {

namespace resources {

static inline int scale(mad_fixed_t sample) {
    // round
    sample += (1L << (MAD_F_FRACBITS - 16));

    // clip
    if (sample >= MAD_F_ONE) {
        sample = MAD_F_ONE - 1;
    } else if (sample < -MAD_F_ONE) {
        sample = -MAD_F_ONE;
    }

    // quantize
    return sample >> (MAD_F_FRACBITS + 1 - 16);
}

void Mp3File::load(ByteArray &&data) {
    _input = data;
    _stream = std::make_shared<AudioStream>();

    mad_decoder decoder;
    mad_decoder_init(
        &decoder,
        this,
        inputFunc,
        headerFunc,
        nullptr,
        outputFunc,
        nullptr,
        nullptr);

    mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);
    mad_decoder_finish(&decoder);
}

mad_flow Mp3File::inputFunc(void *playbuf, mad_stream *stream) {
    Mp3File *mp3 = reinterpret_cast<Mp3File *>(playbuf);
    if (mp3->_done) {
        return MAD_FLOW_BREAK;
    }
    mad_stream_buffer(stream, reinterpret_cast<unsigned char *>(&mp3->_input[0]), mp3->_input.size());
    mp3->_done = true;

    return MAD_FLOW_CONTINUE;
}

mad_flow Mp3File::headerFunc(void *playbuf, mad_header const *header) {
    return MAD_FLOW_CONTINUE;
}

mad_flow Mp3File::outputFunc(void *playbuf, mad_header const *header, mad_pcm *pcm) {
    Mp3File *mp3 = reinterpret_cast<Mp3File *>(playbuf);
    int sampleCount = pcm->length;
    mad_fixed_t *chLeft = pcm->samples[0];
    mad_fixed_t *chRight = pcm->samples[1];

    AudioStream::Frame frame;
    frame.format = pcm->channels == 2 ? AudioFormat::Stereo16 : AudioFormat::Mono16;
    frame.sampleRate = pcm->samplerate;
    frame.samples.reserve(pcm->channels * sampleCount * sizeof(int16_t));

    while (sampleCount--) {
        int sample = scale(*chLeft++);
        frame.samples.push_back((sample >> 0) & 0xff);
        frame.samples.push_back((sample >> 8) & 0xff);
        if (pcm->channels == 2) {
            sample = scale(*chRight++);
            frame.samples.push_back((sample >> 0) & 0xff);
            frame.samples.push_back((sample >> 8) & 0xff);
        }
    }

    mp3->_stream->add(std::move(frame));

    return MAD_FLOW_CONTINUE;
}

std::shared_ptr<AudioStream> Mp3File::stream() const {
    return _stream;
}

} // namespace resources

} // namespace reone
