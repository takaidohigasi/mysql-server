/* Copyright (c) 2013, Oracle and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   51 Franklin Street, Suite 500, Boston, MA 02110-1335 USA */

#include "my_global.h"
#include <gcs_replication.h>
#include <mysqld.h>
#include <log.h>
#include <rpl_info_factory.h>
#include <rpl_slave.h>

Gcs_replication_handler::Gcs_replication_handler() :
  plugin(NULL), plugin_handle(NULL)
{
  plugin_name.str= (char*) "gcs_replication_plugin";
  plugin_name.length= 22;
}

Gcs_replication_handler::~Gcs_replication_handler()
{
  if (plugin_handle)
    plugin_handle->gcs_rpl_stop();
}

int Gcs_replication_handler::gcs_rpl_start()
{
  int error= 0;
  if (!plugin_handle)
    if ((error = gcs_init()))
      return error;
  return plugin_handle->gcs_rpl_start();
}

int Gcs_replication_handler::gcs_rpl_stop()
{
  if (plugin_handle)
    return plugin_handle->gcs_rpl_stop();
  return 1;
}

int Gcs_replication_handler::gcs_init()
{
  plugin= my_plugin_lock_by_name(0, &plugin_name, MYSQL_GCS_RPL_PLUGIN);
  if (plugin)
  {
    plugin_handle= (st_mysql_gcs_rpl *) plugin_decl(plugin)->info;
    plugin_unlock(0, plugin);
  }
  else
  {
    plugin_handle= NULL;
    return 1;
  }
  return 0;
}

Gcs_replication_handler* gcs_rpl_handler= NULL;

int init_gcs_rpl()
{
  if (gcs_rpl_handler != NULL)
    return 1;
  gcs_rpl_handler= new Gcs_replication_handler();
  return 0;
}

int start_gcs_rpl()
{
  if (gcs_rpl_handler)
    return gcs_rpl_handler->gcs_rpl_start();
  return 1;
}

int stop_gcs_rpl()
{
  if (gcs_rpl_handler)
   return gcs_rpl_handler->gcs_rpl_stop();
  return 1;
}

int cleanup_gcs_rpl()
{
  if (gcs_rpl_handler)
    gcs_rpl_handler->gcs_rpl_stop();
  delete gcs_rpl_handler;
  gcs_rpl_handler= NULL;
  return 0;
}

/* Server access methods  */

bool is_server_engine_ready()
{
  return (tc_log != NULL);
}

uint get_opt_mts_checkpoint_group()
{
  return opt_mts_checkpoint_group;
}

ulong get_opt_mts_slave_parallel_workers()
{
  return opt_mts_slave_parallel_workers;
}

ulong get_opt_rli_repository_id()
{
  return opt_rli_repository_id;
}

char *set_relay_log_name(char* name){
  char *original_relaylog_name= opt_relay_logname;
  opt_relay_logname= name;
  return original_relaylog_name;
}

char *set_relay_log_index_name(char* name){
  char *original_relaylog_index_name= opt_relaylog_index_name;
  opt_relaylog_index_name= name;
  return original_relaylog_index_name;
}

char *set_relay_log_info_name(char* name){
  char *original_relay_info_file= relay_log_info_file;
  relay_log_info_file=  name;
  return original_relay_info_file;
}