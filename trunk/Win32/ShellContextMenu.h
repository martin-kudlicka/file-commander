/// Windows shell context menu class
/** slightly modified http://www.codeproject.com/shell/shellcontextmenu.asp
	 developed by R. Engels 2003 */

#include <shlobj.h>
#include <malloc.h>
#include <QStringList>

#if !defined(AFX_SHELLCONTEXTMENU_H__A358AACF_7C7C_410D_AD29_67310B2DDC22__INCLUDED_)
#define AFX_SHELLCONTEXTMENU_H__A358AACF_7C7C_410D_AD29_67310B2DDC22__INCLUDED_

class cShellContextMenu  
{
	public:
		void SetObjects(IShellFolder *psfFolder, LPITEMIDLIST pidlItem);
		void SetObjects(IShellFolder *psfFolder, LPITEMIDLIST *pidlArray, int nItemCount);
		void SetObjects(LPITEMIDLIST pidl);
		void SetObjects(const QString qsObject);			///< set object for context menu
																		/**< \param qsObject object name (filename) */
		void SetObjects(const QStringList qslObjects);	///< set objects for context menu
																		/**< \param qslObjects string list of object names */
		UINT ShowContextMenu (HWND pWnd, POINT pt);		///< show context menu
																		/**< \param pWnd parent window handle
																			  \param pt coordinates of context menu */
		cShellContextMenu();										///< constructor
		virtual ~cShellContextMenu();							///< destructor

	private:
		static const uint MAX_ID = 10000;					///< max ID for context menu command
		static const uint MIN_ID = 1;							///< min ID for context menu command

		int nItems;
		BOOL bDelete;
		HMENU *m_Menu;
		IShellFolder *m_psfFolder;
		LPITEMIDLIST *m_pidlArray;	
		
		void InvokeCommand(LPCONTEXTMENU pContextMenu, UINT idCommand);
		BOOL GetContextMenu(void **ppContextMenu, int &iMenuType);
		HRESULT SHBindToParentEx(LPCITEMIDLIST pidl, REFIID riid, VOID **ppv, LPCITEMIDLIST *ppidlLast);
		static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		void FreePIDLArray(LPITEMIDLIST *pidlArray);
		LPITEMIDLIST CopyPIDL(LPCITEMIDLIST pidl, int cb = -1);
		UINT GetPIDLSize(LPCITEMIDLIST pidl);
		LPBYTE GetPIDLPos(LPCITEMIDLIST pidl, int nPos);
		int GetPIDLCount(LPCITEMIDLIST pidl);
}; // cShellContextMenu

#endif // !defined(AFX_SHELLCONTEXTMENU_H__A358AACF_7C7C_410D_AD29_67310B2DDC22__INCLUDED_)
