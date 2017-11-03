#pragma once

#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/set.hpp>
#include <boost/interprocess/containers/flat_map.hpp>
#include <boost/interprocess/containers/deque.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/sync/interprocess_sharable_mutex.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/file_lock.hpp>

#include <type_traits>

namespace chainbase {

   namespace bip = boost::interprocess;

   template< typename T >
   struct std_allocator_ex : public std::allocator< T >
   {
      using seg_type = decltype( ( (bip::managed_mapped_file*)nullptr )->get_segment_manager() );

      std_allocator_ex()
      {

      }

      std_allocator_ex( seg_type* seg )
      {

      }

      //This method does nothing. It is important to hold consistency with bip::allocator.
      seg_type* get_segment_manager() const
      {
         return nullptr;
      }
   };

   #if ENABLE_STD_ALLOCATOR 
      template< typename T >
      using allocator = std_allocator_ex< T >;
   #else
      template< typename T >
      using allocator = bip::allocator<T, bip::managed_mapped_file::segment_manager>;
   #endif
   // template< typename T >
   // using allocator = typename std::conditional< ENABLE_STD_ALLOCATOR,
   //                            std::allocator< T >,
   //                            bip::allocator<T, bip::managed_mapped_file::segment_manager>
   //                            >::type;

   using shared_string = std::conditional< ENABLE_STD_ALLOCATOR,
                        std::string,
                        bip::basic_string< char, std::char_traits< char >, allocator< char > >
                        >::type;

   template<typename T>
   using t_vector = typename std::conditional< ENABLE_STD_ALLOCATOR,
                              std::vector<T, allocator<T> >,
                              bip::vector<T, allocator<T> >
                              >::type;

   
   template< typename FIRST_TYPE, typename SECOND_TYPE >
   using t_pair = std::pair< FIRST_TYPE, SECOND_TYPE >;

   template< typename FIRST_TYPE, typename SECOND_TYPE >
   using t_allocator_pair = allocator< t_pair< const FIRST_TYPE, SECOND_TYPE > >;

   template< typename KEY_TYPE, typename VALUE_TYPE, typename LESS_FUNC = std::less<KEY_TYPE>>
   using t_flat_map = typename std::conditional< ENABLE_STD_ALLOCATOR,
      boost::container::flat_map< KEY_TYPE, VALUE_TYPE, LESS_FUNC, allocator< t_pair< KEY_TYPE, VALUE_TYPE > > >,
      bip::flat_map< KEY_TYPE, VALUE_TYPE, LESS_FUNC, allocator< t_pair< KEY_TYPE, VALUE_TYPE > > >
      >::type;

   template< typename KEY_TYPE, typename VALUE_TYPE, typename LESS_FUNC = std::less<KEY_TYPE>>
   using t_map = typename std::conditional< ENABLE_STD_ALLOCATOR,
                              std::map< KEY_TYPE, VALUE_TYPE, LESS_FUNC, t_allocator_pair< KEY_TYPE, VALUE_TYPE > >,
                              bip::map< KEY_TYPE, VALUE_TYPE, LESS_FUNC, t_allocator_pair< KEY_TYPE, VALUE_TYPE > >
                              >::type;

   template< typename T >
   using t_deque = typename std::conditional< ENABLE_STD_ALLOCATOR,
                  std::deque< T, allocator< T > >,
                  bip::deque< T, allocator< T > >
                  >::type;

   template< typename T, typename LESS_FUNC >
   using t_set = typename std::conditional< ENABLE_STD_ALLOCATOR,
                  std::set< T, LESS_FUNC, allocator< T > >,
                  bip::set< T, LESS_FUNC, allocator< T >  >
                  >::type;

}