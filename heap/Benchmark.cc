//
// Created by 关鑫 on 2020/9/14.
//

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <queue>
#include <chrono>
#include <random>
#include <limits>
#include <utility>
#include <map>
#include <tuple>
#include <variant>

#include <boost/heap/pairing_heap.hpp>
#include <boost/heap/fibonacci_heap.hpp>
#include <boost/heap/priority_queue.hpp>
#include <boost/heap/d_ary_heap.hpp>

#include "utils/AllocatorTracker.h"

#include "PriorityQueueU.h"

namespace TestMemory {

class RandomData {
public:
    explicit RandomData( int size ) :
            _data( size ) {
        std::random_device rd;
        std::mt19937 g( rd() );


        for( int i = 0; i < size; ++i ) _data.emplace_back( i );
        std::shuffle( _data.begin(), _data.end(), g );
    }

    inline int Get( int pos ) { return _data[pos]; }

    inline int Size() { return _data.size(); }

private:
    std::vector< int > _data;
};

RandomData randomData( 100000 );
constexpr auto MAX_DOUBLE = std::numeric_limits< double >::max();

template < int width, int height >
class RandomMap {
public:
    RandomMap() {
        Reset();
    }

    void Reset() {

        for( int y = 0; y < height; ++y ) {
            for( int x = 0; x < width; ++x ) {
                At( x, y ) = MAX_DOUBLE;
            }
        }

        for( int x = 0; x < width; ++x ) At( x, 0 ) = At( x, height - 1 ) = -1;
        for( int y = 0; y < height; ++y ) At( 0, y ) = At( width - 1, y ) = -1;
        // for( int y = 100; y < 1000; ++y ) At( 512, y ) = -1;
        for( int y = 0; y < height; ++y ) At( 512, y ) = -1;
    }

    inline
    double Get( int x, int y ) const { return _data[y * width + x]; }

    inline
    double &At( int x, int y ) { return _data[y * width + x]; }

    inline
    constexpr int Width() { return width; }

    inline
    constexpr int Height() { return height; }

private:
    std::array< double, width * height > _data;
};

RandomMap< 1024, 1024 > randomMap;

template < typename Point >
class Dijkstra {

public:
    template < typename Q >
    bool Search( const Point &start, const Point &goal, Q &q ) {
        int mSize = 0;
        constexpr int DIR[4][2] = { { -1, 0 },
                                    { 1,  0 },
                                    { 0,  -1 },
                                    { 0,  1 } };
        randomMap.At( start.x, start.y ) = 0;
        q.push( { 0.0, start } );
        while( q.size() ) {
            auto[dist, point] = q.top();
            if( point == goal ) return true;
            q.pop();
            if( dist == randomMap.Get( point.x, point.y ) ) {
                for( int i = 0; i < 4; ++i ) {
                    auto nx = point.x + DIR[i][0];
                    auto ny = point.y + DIR[i][1];
                    auto &oldDist = randomMap.At( nx, ny );
                    auto newDist = dist + 1;
                    if( newDist < oldDist ) {
                        oldDist = newDist;
                        q.push( { newDist, { nx, ny } } );
                    }
                }
            }
            mSize = std::max( mSize, (int)q.size() );
        }
//        printf( "mSize = %d\n", mSize );
        return false;
    }

    template < typename Q >
    bool SearchWithDec( const Point &start, const Point &goal, Q &q ) {
        int mSize = 0;
        typedef typename Q::handle_type Handle;
        std::map< Point, Handle, std::less< Point >, AllocatorTracker< std::pair< const Point, Handle > > > handleMap;
        constexpr int DIR[4][2] = { { -1, 0 },
                                    { 1,  0 },
                                    { 0,  -1 },
                                    { 0,  1 } };
        randomMap.At( start.x, start.y ) = 0;
        handleMap.emplace( start, q.push( { 0.0, start } ) );
        while( q.size() ) {
            auto[dist, point] = q.top();
            // printf( "process %d %d\n", point.x, point.y );
            if( point == goal ) return true;
            q.pop();
            handleMap.erase( point );

            for( int i = 0; i < 4; ++i ) {
                auto nx = point.x + DIR[i][0];
                auto ny = point.y + DIR[i][1];
                auto &oldDist = randomMap.At( nx, ny );
                auto newDist = dist + 1;
                if( newDist < oldDist ) {
                    Point npoint{ nx, ny };
                    auto it = handleMap.find( npoint );
                    oldDist = newDist;
                    if( it == handleMap.end() ) {
                        handleMap.emplace( npoint, q.push( { newDist, npoint } ) );
                    }
                    else {
                        q.update( it->second, { newDist, npoint } );
                    }
                }
            }
            mSize = std::max( mSize, (int)q.size() );
        }
//        printf( "mSize = %d\n", mSize );
        printf( "%-20s%-20d%-20d\n",
                "  |-->handleMap",
                handleMap.get_allocator().get_peak_size(), handleMap.get_allocator().get_cur_size() );
        return false;
    }

    template < typename Q >
    bool SearchWithDec2( const Point &start, const Point &goal, Q &q ) {
        int mSize = 0;
        constexpr int DIR[4][2] = { { -1, 0 },
                                    { 1,  0 },
                                    { 0,  -1 },
                                    { 0,  1 } };
        randomMap.At( start.x, start.y ) = 0;
        q.push( { 0.0, start } );
        while( q.size() ) {
            auto [dist, point] = q.top();
            // printf( "process %d %d\n", point.x, point.y );
            if( point == goal ) return true;
            q.pop();

            for( int i = 0; i < 4; ++i ) {
                auto nx = point.x + DIR[i][0];
                auto ny = point.y + DIR[i][1];
                auto &oldDist = randomMap.At( nx, ny );
                auto newDist = dist + 1;
                if( newDist < oldDist ) {
                    Point npoint{ nx, ny };
                    oldDist = newDist;
                    if( !q.contains( { 0, npoint } ) ) {
                        q.push( { newDist, npoint } );
                    }
                    else {
                        q.update( { newDist, npoint } );
                    }
                }
            }
            mSize = std::max( mSize, (int)q.size() );
        }
//        printf( "mSize = %d\n", mSize );
        return false;
    }

//    template< typename Q >
//    bool SearchWithDec( const Point &start, const Point &goal, Q &q ) {
//        typedef typename Q::handle_type Handle;
//        std::map< Point, Handle > handleMap;
//        constexpr int DIR[4][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };
//        randomMap.At( start.x, start.y ) = 0;
//        handleMap.emplace( start, q.push( start ) );
//        while( q.size() ) {
//            auto point = q.top();
//            // printf( "process %d %d\n", point.x, point.y );
//            if( point == goal ) return true;
//            q.pop();
//
//            auto curDist = randomMap.Get( point.x, point.y );
//            for( int i = 0; i < 4; ++i ) {
//                auto nx = point.x + DIR[i][0];
//                auto ny = point.y + DIR[i][1];
//                auto &oldDist = randomMap.At( nx, ny );
//                auto newDist = curDist + 1;
//                if( newDist < oldDist ) {
//                    Point npoint{ nx, ny };
//                    auto it = handleMap.find( npoint );
//                    oldDist = newDist;
//                    if( it == handleMap.end() ) {
//                        handleMap.emplace( npoint, q.push( npoint ) );
//                    }
//                    else {
//                        q.update( it->second );
//                    }
//                }
//            }
//        }
//        return false;
//    }
};

struct Point2D {
    Point2D() {}

    Point2D( int x, int y ) : x( x ), y( y ) {}

    Point2D( const Point2D &other ): x( other.x ), y( other.y ) {}

    bool operator==( const Point2D &other ) const { return x == other.x && y == other.y; }

    bool operator<( const Point2D &other ) const { return x < other.x || ( x == other.x && y < other.y ); }

    bool operator>( const Point2D &other ) const { return x > other.x || ( x == other.x && y > other.y ); }

    int x;
    int y;
};

struct HashPoint2D {
    size_t operator()( const Point2D &p ) const {
        return ( p.x << 16 ) | ( p.y & 0xFFFF );
    }
};

typedef std::pair< double, Point2D > DistPoint;

struct CmpPoint2D {
    bool operator()( const Point2D &a, const Point2D &b ) const {
        return randomMap.At( a.x, a.y ) > randomMap.At( b.x, b.y );
    }
};

struct CmpDistPoint {
    bool operator()( const DistPoint &a, const DistPoint &b ) const {
        return a.first > b.first || ( a.first == b.first && a.second > b.second );
    }
};


enum class Op {
    PUSH,
    PUSH_POP,
    DIJKSTRA,
    DIJKSTRA_DEC
};

template < typename T >
long CommonTest( Op op, T &q ) {

    auto beg = std::chrono::high_resolution_clock::now();

    if( op == Op::PUSH ) {
        for( int i = 0; i < randomData.Size(); ++i ) q.push( randomData.Get( i ) );
    }
    else if( op == Op::PUSH_POP ) {
        for( int i = 0; i < randomData.Size(); ++i ) q.push( randomData.Get( i ) );
        while( q.size() ) {
            q.top();
            q.pop();
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast< std::chrono::microseconds >( end - beg ).count();
}

using std::declval;

template < class T, class = std::void_t< >>
struct HasUpdateFunction : std::false_type {
};

template < class T >
struct HasUpdateFunction<T, std::void_t<decltype(declval<T>().update( declval<typename T::handle>() ))>> : std::true_type {};


template < typename T >
long CommonTestDij( T &q ) {
    randomMap.Reset();
    auto beg = std::chrono::high_resolution_clock::now();

    Dijkstra< Point2D > dij;
    dij.Search( { 20, 100 }, { 700, 750 }, q );

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast< std::chrono::microseconds >( end - beg ).count();
}

template < typename T >
long CommonTestDijWithDec( T &q ) {

    randomMap.Reset();

    auto beg = std::chrono::high_resolution_clock::now();

    Dijkstra< Point2D > dij;
    dij.SearchWithDec( { 20, 100 }, { 700, 750 }, q );

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast< std::chrono::microseconds >( end - beg ).count();
}

template < typename T >
long CommonTestDijWithDec2( T &q ) {

    randomMap.Reset();

    auto beg = std::chrono::high_resolution_clock::now();

    Dijkstra< Point2D > dij;
    dij.SearchWithDec2( { 20, 100 }, { 700, 750 }, q );

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast< std::chrono::microseconds >( end - beg ).count();
}

void TestPairingHeap( Op op ) {
    typedef boost::heap::pairing_heap< int,
            boost::heap::compare< std::greater< int > >,
            boost::heap::allocator< AllocatorTracker< int > >
    > PriorityQ;
    PriorityQ q;
    auto dur = CommonTest( op, q );
    printf( "%-20s%-20d%-20d%-20ld\n",
            "BoostPairingHeap",
            q.get_allocator().get_peak_size(), q.get_allocator().get_cur_size(), dur );
}

void TestPairingHeapDij() {
    typedef boost::heap::pairing_heap< DistPoint,
            boost::heap::compare< CmpDistPoint >,
            boost::heap::allocator< AllocatorTracker< DistPoint > >
    > PriorityQ;
    PriorityQ q;
    auto dur = CommonTestDij( q );
    printf( "%-20s%-20d%-20d%-20ld\n",
            "BoostPairingHeap",
            q.get_allocator().get_peak_size(), q.get_allocator().get_cur_size(), dur );
}

void TestPairingHeapDijWithDec() {
    typedef boost::heap::pairing_heap< DistPoint,
            boost::heap::compare< CmpDistPoint >,
            boost::heap::allocator< AllocatorTracker< DistPoint > >
    > PriorityQ;
    PriorityQ q;
    auto dur = CommonTestDijWithDec( q );
    printf( "%-20s%-20d%-20d%-20ld\n",
            "BoostPairingHeap",
            q.get_allocator().get_peak_size(), q.get_allocator().get_cur_size(), dur );
}

void TestFibonacciHeap( Op op ) {
    typedef boost::heap::fibonacci_heap< int,
            boost::heap::allocator< AllocatorTracker< int > >
    > PriorityQ;
    PriorityQ q;
    auto dur = CommonTest( op, q );
    printf( "%-20s%-20d%-20d%-20ld\n",
            "BoostFibonacciHeap",
            q.get_allocator().get_peak_size(), q.get_allocator().get_cur_size(), dur );
}

void TestFibonacciHeapDij() {
    typedef boost::heap::fibonacci_heap< DistPoint,
            boost::heap::compare< CmpDistPoint >,
            boost::heap::allocator< AllocatorTracker< DistPoint > >
    > PriorityQ;
    PriorityQ q;
    auto dur = CommonTestDij( q );
    printf( "%-20s%-20d%-20d%-20ld\n",
            "BoostFibonacciHeap",
            q.get_allocator().get_peak_size(), q.get_allocator().get_cur_size(), dur );
}

void TestFibonacciHeapDijWithDec() {
    typedef boost::heap::fibonacci_heap< DistPoint,
            boost::heap::compare< CmpDistPoint >,
            boost::heap::allocator< AllocatorTracker< DistPoint > >
    > PriorityQ;
    PriorityQ q;
    auto dur = CommonTestDijWithDec( q );
    printf( "%-20s%-20d%-20d%-20ld\n",
            "BoostFibonacciHeap",
            q.get_allocator().get_peak_size(), q.get_allocator().get_cur_size(), dur );
}

void TestDaryHeap( Op op ) {
    typedef boost::heap::d_ary_heap< int,
            boost::heap::arity< 4 >,
            boost::heap::stable< true >,
            boost::heap::compare< std::less< int > >,
            boost::heap::allocator< AllocatorTracker< int > >,
            boost::heap::mutable_< true >
    > PriorityQ;
    PriorityQ q;
    auto dur = CommonTest( op, q );
    printf( "%-20s%-20d%-20d%-20ld\n",
            "BoostDaryHeap",
            q.get_allocator().get_peak_size(), q.get_allocator().get_cur_size(), dur );
}

void TestDaryHeapDij() {
    typedef boost::heap::d_ary_heap< DistPoint,
            boost::heap::arity< 4 >,
            boost::heap::stable< true >,
            boost::heap::compare< CmpDistPoint >,
            boost::heap::allocator< AllocatorTracker< DistPoint > >,
            boost::heap::mutable_< true >
    > PriorityQ;
    PriorityQ q;
    auto dur = CommonTestDij( q );
    printf( "%-20s%-20d%-20d%-20ld\n",
            "BoostDaryHeap",
            q.get_allocator().get_peak_size(), q.get_allocator().get_cur_size(), dur );
}

void TestDaryHeapDijWithDec() {
    typedef boost::heap::d_ary_heap< DistPoint,
            boost::heap::arity< 4 >,
            boost::heap::stable< true >,
            boost::heap::compare< CmpDistPoint >,
            boost::heap::allocator< AllocatorTracker< DistPoint > >,
            boost::heap::mutable_< true >
    > PriorityQ;
    PriorityQ q;
    auto dur = CommonTestDijWithDec( q );
    printf( "%-20s%-20d%-20d%-20ld\n",
            "BoostDaryHeap",
            q.get_allocator().get_peak_size(), q.get_allocator().get_cur_size(), dur );
}

void TestPriorityQueue( Op op ) {
    typedef boost::heap::priority_queue< int,
            AllocatorTracker< int >
    > PriorityQ;
    PriorityQ q;
    auto dur = CommonTest( op, q );
    printf( "%-20s%-20d%-20d%-20ld\n",
            "BoostPriorityQueue",
            q.get_allocator().get_peak_size(), q.get_allocator().get_cur_size(), dur );
}

void TestPriorityQueueDij() {
    typedef boost::heap::priority_queue< DistPoint,
            boost::heap::compare< CmpDistPoint >,
            boost::heap::allocator< AllocatorTracker< DistPoint > >
    > PriorityQ;
    PriorityQ q;
    auto dur = CommonTestDij( q );
    printf( "%-20s%-20d%-20d%-20ld\n",
            "BoostPriorityQueue",
            q.get_allocator().get_peak_size(), q.get_allocator().get_cur_size(), dur );
}

void TestStdPriorityQueue( Op op ) {
    typedef std::priority_queue< int,
            std::deque< int, AllocatorTracker< int > >
    > PriorityQ;
    AllocatorTracker< int > tracker;
    PriorityQ q( tracker );
    auto dur = CommonTest( op, q );
    printf( "%-20s%-20d%-20d%-20ld\n",
            "StdPriorityQueue",
            tracker.get_peak_size(), tracker.get_cur_size(), dur );
}

void TestStdPriorityQueueDij() {
    typedef std::priority_queue< DistPoint,
            std::deque< DistPoint, AllocatorTracker< DistPoint > >,
            CmpDistPoint
    > PriorityQ;
    AllocatorTracker< DistPoint > tracker;
    PriorityQ q( tracker );
    auto dur = CommonTestDij( q );
    printf( "%-20s%-20d%-20d%-20ld\n",
            "StdPriorityQueue",
            tracker.get_peak_size(), tracker.get_cur_size(), dur );
}

typedef std::unordered_map< Point2D, int32_t, HashPoint2D, std::equal_to<Point2D >, AllocatorTracker< std::pair< const Point2D, int32_t > > > LocMap;

struct Key : public robot::PquKeyBase {
    double dist;
    Point2D point;

    Key( double dist, const Point2D &point ) :
            robot::PquKeyBase(), dist( dist ), point( point ) {};

    Key( const Key &other ) :
            robot::PquKeyBase( other ),
            dist( other.dist ),
            point( other.point ) {}

    Key( Key &&other ) :
            robot::PquKeyBase( std::move( other ) ),
            dist( std::move( other.dist ) ),
            point( std::move( other.point ) ) {}

    bool operator<( const Key &other ) const {
        return dist < other.dist || ( dist == other.dist && point < other.point );
    }

    bool operator>( const Key &other ) const {
        return dist > other.dist || ( dist == other.dist && point > other.point );
    }

    int32_t getLoc() {
        if( !locator ) return -1;
        auto &locMap = *(LocMap *) locator;
        auto it = locMap.find( point );
        return it == locMap.end() ? -1 : it->second;
    }

    void setLoc( int32_t loc ) {
        this->loc = loc;
        if( locator ) ( *(LocMap *) locator )[point] = this->loc;
    }

    void eraseLoc() {
        if( locator ) ( (LocMap *) locator )->erase( point );
        this->loc = -1;
        this->locator = nullptr;
    }

    void setLocator( void *locator ) {
        this->locator = locator;
    }

    Key &operator=( Key &&other ) {
        dist = other.dist;
        point = other.point;
        if( locator ) ( *(LocMap *) locator )[point] = this->loc;
        return *this;
    }
};

template <size_t I> auto get( const Key& k ) {
    if constexpr (I == 0) return k.dist;
    else if constexpr (I == 1) return k.point;
}

}

namespace std {
template <> struct tuple_size<TestMemory::Key> : integral_constant<size_t, 2> { };
template <> struct tuple_element<0,TestMemory::Key> { using type = double; };
template <> struct tuple_element<1,TestMemory::Key> { using type = TestMemory::Point2D; };
}

namespace TestMemory {

struct KeyCmp {
    bool operator()( const Key &a, const Key &b ) const { return a > b; }
};

void TestPquDijWithDec() {
    typedef AllocatorTracker< Key > PquTracker;
    typedef robot::PriorityQueueU< Key, std::deque< Key, PquTracker >, KeyCmp > PriorityQ;
    PquTracker tracker;
    LocMap locMap;
    PriorityQ q(&locMap, tracker);
    auto dur = CommonTestDijWithDec2( q );
    printf( "%-20s%-20d%-20d\n", "  |-->handleMap", locMap.get_allocator().get_peak_size(), locMap.get_allocator().get_cur_size() );
    printf( "%-20s%-20d%-20d%-20ld\n",
            "PriorityQueueU",
            tracker.get_peak_size(), tracker.get_cur_size(), dur );
}



void Run() {

    printf( "\033[32m============ Push Compare =============\033[m\n" );
    printf( "%-20s%-20s%-20s%-20s\n", "Type", "MaxMem", "CurMem", "Time" );

    TestPriorityQueue( Op::PUSH );
    TestStdPriorityQueue( Op::PUSH );
    TestDaryHeap( Op::PUSH );
    TestFibonacciHeap( Op::PUSH );
    TestPairingHeap( Op::PUSH );

    printf( "\n\n" );

    printf( "\033[32m============ PushPop Compare =============\033[m\n" );
    printf( "%-20s%-20s%-20s%-20s\n", "Type", "MaxMem", "CurMem", "Time" );

    TestPriorityQueue( Op::PUSH_POP );
    TestStdPriorityQueue( Op::PUSH_POP );
    TestDaryHeap( Op::PUSH_POP );
    TestFibonacciHeap( Op::PUSH_POP );
    TestPairingHeap( Op::PUSH_POP );

    printf( "\n\n" );

    printf( "\033[32m============ Dijkstra Compare =============\033[m\n" );
    printf( "%-20s%-20s%-20s%-20s\n", "Type", "MaxMem", "CurMem", "Time" );

    TestPriorityQueueDij();
    TestStdPriorityQueueDij();
    TestDaryHeapDij();
    TestFibonacciHeapDij();
    TestPairingHeapDij();

    printf( "\n\n" );


    printf( "\033[32m============ Dijkstra With Dec Compare =============\033[m\n" );
    printf( "%-20s%-20s%-20s%-20s\n", "Type", "MaxMem", "CurMem", "Time" );

    TestDaryHeapDijWithDec();
    TestFibonacciHeapDijWithDec();
    TestPairingHeapDijWithDec();
    TestPquDijWithDec();

    printf( "\n\n" );

}

}

int main() {

    TestMemory::Run();

    return 0;
}