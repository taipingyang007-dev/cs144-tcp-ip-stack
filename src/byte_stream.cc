#include "byte_stream.hh"
#include <cstddef>

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

void Writer::push( string data )
{
  // Your code here.
  uint64_t actual = min( static_cast<uint64_t>( data.size() ), available_capacity() );
  if ( actual == 0 )
    return;
  data.resize( actual );
  buffer_.emplace( move( data ) );
  bytes_pushed_ += actual;
}

void Writer::close()
{
  closed_ = true; // Your code here.
}

bool Writer::is_closed() const
{
  return closed_; // Your code here.
}

uint64_t Writer::available_capacity() const
{
  return capacity_ - ( bytes_pushed_ - bytes_popped_ ); // Your code here.
}

uint64_t Writer::bytes_pushed() const
{
  return bytes_pushed_; // Your code here.
}

string_view Reader::peek() const
{
  string_view str_v {};
  if ( buffer_.empty() )
    return str_v;

  str_v = buffer_.front();
  return str_v.substr( offset_ );
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  uint64_t actual = min( len, bytes_pushed_ - bytes_popped_ );

  // 开始删除，如果删除在一个块内应该只移动offset，如果当前块删除完，应该 pop()
  while ( actual > 0 ) {
    if ( buffer_.front().size() - offset_ >= actual ) {
      // 说明当前块内可以直接删除
      offset_ += actual;
      bytes_popped_ += actual;
      actual = 0;

      if ( buffer_.front().size() == offset_ ) {
        offset_ = 0;
        buffer_.pop();
      }
    } else {
      // 当前块内不能直接删除
      actual -= ( buffer_.front().size() - offset_ );
      bytes_popped_ += buffer_.front().size() - offset_;
      offset_ = 0;
      buffer_.pop();
    }
  }
}

bool Reader::is_finished() const
{
  return closed_ && buffer_.empty(); // Your code here.
}

uint64_t Reader::bytes_buffered() const
{
  return bytes_pushed_ - bytes_popped_; // Your code here.
}

uint64_t Reader::bytes_popped() const
{
  return bytes_popped_; // Your code here.
}
