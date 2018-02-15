#pragma once

#include <steem/protocol/types.hpp>
#include <steem/chain/steem_object_types.hpp>
#include <steem/chain/database.hpp>

#include <sstream>
#include <set>

namespace steem { namespace plugins{ namespace follow {

using namespace steem::chain;
using steem::chain::database;
 
using steem::protocol::account_name_type;

class performance_impl;

class dumper
{
   private:

      uint min_block = 19860000;
      uint block = 0;

      std::set< string > last;
      std::ofstream f;

      static std::unique_ptr< dumper > self;

      dumper() :
#if ENABLE_STD_ALLOCATOR == 1
      f( "std_dumped_objects.txt", std::ofstream::out | std::ofstream::app )
#else
      f( "bip_dumped_objects.txt", std::ofstream::out | std::ofstream::app )
#endif
      {
      }

   public:

      ~dumper()
      {
         f.flush();
         f.close();
      }

      static std::unique_ptr< dumper >& instance()
      {
         if( !self )
            self = std::unique_ptr< dumper >( new dumper() );

         return self;
      }

      void clear_strings()
      {
         last.clear();
      }

      void check_block( uint32_t _block )
      {
         block = _block;
      }

      template< typename T, typename T2 >
      void dump( const char* message, const T& data, const T2& data2 )
      {
         static uint64_t counter = 0;

         if( block >= min_block )
         {
            std::stringstream s;
            s<<" "<<message<<" "<<data<<" "<<data2<<"\n";
            std::string tmp_str = s.str();

            //if( last.find( tmp_str ) == last.end() )
            {
               last.insert( tmp_str );
               s.str("");
               s<<counter++<<" "<<message<<" "<<data<<" "<<data2<<"\n";
               tmp_str = s.str();
               f<<tmp_str;
               f.flush();
            }
         }
      }

      template< typename T, typename T2, typename T3 >
      void dump( const char* message, const T& data, const T2& data2, const T3& data3 )
      {
         static uint64_t counter = 0;

         if( block >= min_block )
         {
            std::stringstream s;
            s<<" "<<message<<" "<<data<<" "<<data2<<"\n";
            std::string tmp_str = s.str();

            //if( last.find( tmp_str ) == last.end() )
            {
               last.insert( tmp_str );
               s.str("");
               s<<counter++<<" "<<message<<" "<<data<<" "<<data2<<" "<<data3<<"\n";
               tmp_str = s.str();
               f<<tmp_str;
               f.flush();
            }
         }
      }

      template< typename T, typename T2, typename T3, typename T4 >
      void dump( const char* message, const T& data, const T2& data2, const T3& data3, const T4& data4 )
      {
         static uint64_t counter = 0;

         if( block >= min_block )
         {
            std::stringstream s;
            s<<" "<<message<<" "<<data<<" "<<data2<<"\n";
            std::string tmp_str = s.str();

            //if( last.find( tmp_str ) == last.end() )
            {
               last.insert( tmp_str );
               s.str("");
               s<<counter++<<" "<<message<<" "<<data<<" "<<data2<<" "<<data3<<" "<<data4<<"\n";
               tmp_str = s.str();
               f<<tmp_str;
               f.flush();
            }
         }
      }
};

struct performance_data
{
   enum t_creation_type{ none = 0, full_feed, part_feed, full_blog };

   const account_name_type* account = nullptr;
   const time_point_sec* time = nullptr;
   const comment_id_type* comment = nullptr;

   uint32_t old_id = 0;

   struct
   {
      unsigned creation : 1;
      bool is_empty     : 1;
      bool allow_modify : 1;
      bool allow_delete : 1;
   } s;

   performance_data()
   {
      memset( &s, 0, sizeof( s ) );
   }

   performance_data( const comment_id_type& _comment, bool _is_empty )
   {
      init( _comment, _is_empty );
   }

   void init( const account_name_type& _account, const time_point_sec& _time, const comment_id_type& _comment, bool _is_empty, uint32_t _old_id )
   {
      account = &_account;
      time = &_time;
      comment = &_comment;
      s.creation = true;
      s.is_empty = _is_empty;

      old_id = _old_id;
      s.allow_modify = true;
      s.allow_delete = true;
   }

   void init( const comment_id_type& _comment, bool _is_empty )
   {
      account = nullptr;
      time = nullptr;
      comment = &_comment;
      s.creation = true;
      s.is_empty = _is_empty;

      old_id = 0;
      s.allow_modify = true;
      s.allow_delete = _is_empty;
   }
   
};

class performance
{

   private:

      std::unique_ptr< performance_impl > my;

   public:
   
      performance( database& _db );
      ~performance();

      template< performance_data::t_creation_type CreationType, typename Index >
      uint32_t delete_old_objects( const Index& old_idx, const account_name_type& start_account, uint32_t max_size, performance_data& pd ) const;

      template< typename T, typename T2 >
      static void dump( const char* message, const T& data, const T2& data2 )
      {
         dumper::instance()->dump( message, data, data2 );
      }
};

} } } //steem::follow