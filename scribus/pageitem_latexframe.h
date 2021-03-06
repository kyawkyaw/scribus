/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						pageitem_latexframe.h  -  description
						-------------------
copyright            : Scribus Team
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef PAGEITEM_LATEXFRAME_H
#define PAGEITEM_LATEXFRAME_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QProcess>
#include <QMap>

#include "scribusapi.h"
#include "pageitem.h"
#include "pageitem_imageframe.h"

class QTemporaryFile;
class LatexEditor;
class LatexConfigParser;
class QTimer;

/** @brief Displays all kinds of content generated by external programs.
	Named LatexFrame because it initially only showed the output of Latex.
*/
class SCRIBUS_API PageItem_LatexFrame : public PageItem_ImageFrame
{
	Q_OBJECT

	public:
		PageItem_LatexFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, const QString& fill, const QString& outline);
		~PageItem_LatexFrame();
		
		virtual PageItem_LatexFrame * asLatexFrame() { return this; }
		virtual bool isLatexFrame() const { return true; }

		virtual void clearContents();
		virtual ItemType realItemType() const { return PageItem::LatexFrame; }
		virtual void applicableActions(QStringList& actionList);
		virtual QString infoDescription();
		void layout();
		
		/** @brief Perform undo/redo action */
		void restore(UndoState *state, bool isUndo);
		
		
		/** @brief UI-Callback that runs the editor. */
		void runEditor();
		/*TODO void convertToVector(); */
		
		/** @brief Sets the formula text and forces rerunning latex.
			Emits formulaAutoUpdate() when undoable is false.
			@return Returns true if the frame has to be updated, false if nothing changed.
		*/
		bool setFormula(QString formula, bool undoable=true);
		/** @brief Get current source. */
		QString formula() const { return formulaText; }
		
		/** @brief Runs the external application and sets internal vars and loads
		the image.*/
		void runApplication();
		
		void rerunApplication(bool updateDisplay=false);
		
		QString configFile() const;
		void setConfigFile(QString newConfig, bool relative=false);
		QString application() const;
		bool usePreamble() const { return m_usePreamble; }
		int dpi() const { return m_dpi; }
		int realDpi() const;
		
		const QString output() const { return appStdout; }
		QProcess::ProcessState state() const { return latex->state(); }
		int error() const { return m_err; }

		QMap<QString,QString> editorProperties;
	protected:
		virtual void DrawObj_Item(ScPainter *p, QRectF e);
		double m_lastWidth, m_lastHeight, m_lastDpi;
		
		QString formulaText;

		void writeFileContents(QFile *tempfile);
		void deleteImageFile();
		/* Last error code */
		int m_err;
		int m_dpi;
		
		QString imageFile, tempFileBase;
		QString appStdout;
		QString configFilename;
		
		QProcess *latex;
		LatexEditor *internalEditor;
		LatexConfigParser *config;
		bool m_imgValid;
		bool m_usePreamble;
		bool m_killed;
	signals:
		void formulaAutoUpdate(QString oldText, QString newText);
		void latexFinished();
		void stateChanged(QProcess::ProcessState state);
		void applicationChanged();
	protected slots:
		void updateImage(int exitCode, QProcess::ExitStatus exitStatus);
		void latexError(QProcess::ProcessError error);
	public slots:
		void killProcess();
		void setDpi(int dpi);
		void setUsePreamble(bool);
};

#endif
