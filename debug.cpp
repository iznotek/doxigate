/******************************************************************************
 *
 * $Id: debug.cpp,v 1.7 2001/03/19 19:27:40 root Exp $
 *
 * Copyright (C) 1997-2008 by Dimitri van Heesch.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License is hereby
 * granted. No representations are made about the suitability of this software
 * for any purpose. It is provided "as is" without express or implied warranty.
 * See the GNU General Public License for more details.
 *
 * Documents produced by Doxygen are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
 */

#include <stdarg.h>
#include <stdio.h>

#include <QHash>
//#include <Q3CString>
#include <QtCore>

#include "debug.h"

//------------------------------------------------------------------------

struct LabelMap
{
  const char *name;
  Debug::DebugMask event;
};

static LabelMap s_labels[] =
{
  { "findmembers",  Debug::FindMembers  },
  { "functions",    Debug::Functions    },
  { "variables",    Debug::Variables    },
  { "preprocessor", Debug::Preprocessor },
  { "classes",      Debug::Classes      },
  { "commentcnv",   Debug::CommentCnv   },
  { "commentscan",  Debug::CommentScan  },
  { "validate",     Debug::Validate     },
  { "printtree",    Debug::PrintTree    },
  { "time",         Debug::Time         },
  { "extcmd",       Debug::ExtCmd       },
  { 0,             (Debug::DebugMask)0  }
};

class LabelMapper
{
  public:
    LabelMapper() : m_map()
    {
      m_map.reserve(17);
      //m_map.setAutoDelete(true);
      LabelMap *p = s_labels;
      while (p->name)
      {
        m_map.insert(p->name,new Debug::DebugMask(p->event));
        p++;
      }
    }
    Debug::DebugMask *find(const QString& s) const
    {
      if (s==0) return 0;
      return m_map.find(s).value();
    }
  private:
    QHash<QString,Debug::DebugMask*> m_map;
};

static LabelMapper g_labelMapper;

//------------------------------------------------------------------------

Debug::DebugMask Debug::curMask = Debug::Quiet;
int Debug::curPrio = 0;

void Debug::print(DebugMask mask,int prio,const char *fmt,...)
{
  if ((curMask&mask) && prio<=curPrio)
  {
    va_list args;
    va_start(args,fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
  }
}

static int labelToEnumValue(const char *l)
{
  QString label=l;
  Debug::DebugMask *event = g_labelMapper.find(label.toLower());
  if (event) return *event; else return 0;
}

void Debug::setFlag(const char *lab)
{
  curMask = (DebugMask)(curMask | labelToEnumValue(lab));
}

void Debug::clearFlag(const char *lab)
{
  curMask = (DebugMask)(curMask & ~labelToEnumValue(lab));
}

void Debug::setPriority(int p)
{
  curPrio = p;
}

bool Debug::isFlagSet(DebugMask mask)
{
  return (curMask & mask)!=0;
}

