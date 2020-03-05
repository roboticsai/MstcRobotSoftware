
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iomanip>
#include <iostream>
#include <iterator>


const sf::Uint8 audioData   = 1;
const sf::Uint8 endOfStream = 2;


////////////////////////////////////////////////////////////
/// Customized sound stream for acquiring audio data
/// from the network
////////////////////////////////////////////////////////////
class NetworkAudioStream : public sf::SoundStream
{
public:

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    NetworkAudioStream() :
    m_offset     (0),
    m_hasFinished(false)
    {
        // Set the sound parameters
        initialize(1, 4410);
    }

    ////////////////////////////////////////////////////////////
    /// Run the server, stream audio data from the client
    ///
    ////////////////////////////////////////////////////////////
    void start(unsigned short port)
    {
        if (!m_hasFinished)
        {
            // Listen to the given port for incoming connections
            if (m_listener.listen(port) != sf::Socket::Done)
                return;
            std::cout << "Server is listening to port " << port << ", waiting for connections... " << std::endl;

            // Wait for a connection
            if (m_listener.accept(m_client) != sf::Socket::Done)
                return;
            std::cout << "Client connected: " << m_client.getRemoteAddress() << std::endl;

            // Start playback
            play();

            // Start receiving audio data
            receiveLoop();
        }
        else
        {
            // Start playback
            play();
        }
    }

private:

    ////////////////////////////////////////////////////////////
    /// /see SoundStream::OnGetData
    ///
    ////////////////////////////////////////////////////////////
    virtual bool onGetData(sf::SoundStream::Chunk& data)
    {
        // We have reached the end of the buffer and all audio data have been played: we can stop playback
        if ((m_offset >= m_samples.size()) && m_hasFinished)
            return false;

        // No new data has arrived since last update: wait until we get some
        while ((m_offset >= m_samples.size()) && !m_hasFinished)
            sf::sleep(sf::milliseconds(10));

        // Copy samples into a local buffer to avoid synchronization problems
        // (don't forget that we run in two separate threads)
        {
            sf::Lock lock(m_mutex);
            m_tempBuffer.assign(m_samples.begin() + m_offset, m_samples.end());
        }

        // Fill audio data to pass to the stream
        data.samples     = &m_tempBuffer[0];
        data.sampleCount = m_tempBuffer.size();

        // Update the playing offset
        m_offset += m_tempBuffer.size();

        return true;
    }

    ////////////////////////////////////////////////////////////
    /// /see SoundStream::OnSeek
    ///
    ////////////////////////////////////////////////////////////
    virtual void onSeek(sf::Time timeOffset)
    {
        m_offset = timeOffset.asMilliseconds() * getSampleRate() * getChannelCount() / 1000;
    }

    ////////////////////////////////////////////////////////////
    /// Get audio data from the client until playback is stopped
    ///
    ////////////////////////////////////////////////////////////
    void receiveLoop()
    {
        while (!m_hasFinished)
        {
            // Get waiting audio data from the network
            sf::Packet packet;
            if (m_client.receive(packet) != sf::Socket::Done)
                break;

            // Extract the message ID
            sf::Uint8 id;
            packet >> id;

            if (id == audioData)
            {
                // Extract audio samples from the packet, and append it to our samples buffer
                const sf::Int16* samples     = reinterpret_cast<const sf::Int16*>(static_cast<const char*>(packet.getData()) + 1);
                std::size_t      sampleCount = (packet.getDataSize() - 1) / sizeof(sf::Int16);

                // Don't forget that the other thread can access the sample array at any time
                // (so we protect any operation on it with the mutex)
                {
                    sf::Lock lock(m_mutex);
                    std::copy(samples, samples + sampleCount, std::back_inserter(m_samples));
                }
            }
            else if (id == endOfStream)
            {
                // End of stream reached: we stop receiving audio data
                std::cout << "Audio data has been 100% received!" << std::endl;
                m_hasFinished = true;
            }
            else
            {
                // Something's wrong...
                std::cout << "Invalid packet received..." << std::endl;
                m_hasFinished = true;
            }
        }
    }

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    sf::TcpListener        m_listener;
    sf::TcpSocket          m_client;
    sf::Mutex              m_mutex;
    std::vector<sf::Int16> m_samples;
    std::vector<sf::Int16> m_tempBuffer;
    std::size_t            m_offset;
    bool                   m_hasFinished;
};


////////////////////////////////////////////////////////////
/// Launch a server and wait for incoming audio data from
/// a connected client
///
////////////////////////////////////////////////////////////
void doServer(unsigned short port)
{
    // Build an audio stream to play sound data as it is received through the network
    NetworkAudioStream audioStream;
    audioStream.start(port);

    // Loop until the sound playback is finished
    while (audioStream.getStatus() != sf::SoundStream::Stopped)
    {
        // Leave some CPU time for other threads
        sf::sleep(sf::milliseconds(1));
    }

    std::cin.ignore(10000, '\n');

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to replay the sound..." << std::endl;
    std::cin.ignore(10000, '\n');

    // Replay the sound (just to make sure replaying the received data is OK)
    audioStream.play();

    // Loop until the sound playback is finished
    while (audioStream.getStatus() != sf::SoundStream::Stopped)
    {
        // Leave some CPU time for other threads
        sf::sleep(sf::milliseconds(100));
    }
}


////////////////////////////////////////////////////////////
/// Specialization of audio recorder for sending recorded audio
/// data through the network
////////////////////////////////////////////////////////////
class NetworkRecorder : public sf::SoundRecorder
{
public:

    ////////////////////////////////////////////////////////////
    /// Constructor
    ///
    /// \param host Remote host to which send the recording data
    /// \param port Port of the remote host
    ///
    ////////////////////////////////////////////////////////////
    NetworkRecorder(const sf::IpAddress& host, unsigned short port) :
    m_host(host),
    m_port(port)
    {
    }

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    /// \see SoundRecorder::~SoundRecorder()
    ///
    ////////////////////////////////////////////////////////////
    ~NetworkRecorder()
    {
        // Make sure to stop the recording thread
        stop();
    }

private:

    ////////////////////////////////////////////////////////////
    /// \see SoundRecorder::onStart
    ///
    ////////////////////////////////////////////////////////////
    virtual bool onStart()
    {
        if (m_socket.connect(m_host, m_port) == sf::Socket::Done)
        {
            std::cout << "Connected to server " << m_host << std::endl;
            return true;
        }
        else
        {
            return false;
        }
    }

    ////////////////////////////////////////////////////////////
    /// \see SoundRecorder::onProcessSamples
    ///
    ////////////////////////////////////////////////////////////
    virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
    {
        // Pack the audio samples into a network packet
        sf::Packet packet;
        packet << audioData;
        packet.append(samples, sampleCount * sizeof(sf::Int16));

        // Send the audio packet to the server
        return m_socket.send(packet) == sf::Socket::Done;
    }

    ////////////////////////////////////////////////////////////
    /// \see SoundRecorder::onStop
    ///
    ////////////////////////////////////////////////////////////
    virtual void onStop()
    {
        // Send a "end-of-stream" packet
        sf::Packet packet;
        packet << endOfStream;
        m_socket.send(packet);

        // Close the socket
        m_socket.disconnect();
    }

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    sf::IpAddress  m_host;   ///< Address of the remote host
    unsigned short m_port;   ///< Remote port
    sf::TcpSocket  m_socket; ///< Socket used to communicate with the server
};


////////////////////////////////////////////////////////////
/// Create a client, connect it to a running server and
/// start sending him audio data
///
////////////////////////////////////////////////////////////
void doClient(unsigned short port,char *serverName)
{
    // Check that the device can capture audio
    if (!sf::SoundRecorder::isAvailable())
    {
        std::cout << "Sorry, audio capture is not supported by your system" << std::endl;
        return;
    }

    // Ask for server address
    sf::IpAddress server = serverName;

    // Create an instance of our custom recorder
    NetworkRecorder recorder(server, port);

    // Start capturing audio data
    recorder.start(4410);
    std::cout << "Recording... press enter to stop";
    std::cin.ignore(10000, '\n');
    recorder.stop();
}
