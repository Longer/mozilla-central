/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla XForms support.
 *
 * The Initial Developer of the Original Code is
 * IBM Corporation.
 * Portions created by the Initial Developer are Copyright (C) 2004
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *  Brian Ryner <bryner@brianryner.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef nsXFormsModelElement_h_
#define nsXFormsModelElement_h_

#include "nsXFormsStubElement.h"
#include "nsIModelElementPrivate.h"
#include "nsIDOMEventListener.h"
#include "nsISchema.h"
#include "nsCOMArray.h"
#include "nsVoidArray.h"
#include "nsCOMPtr.h"
#include "nsIDOMDocument.h"
#include "nsXFormsMDGEngine.h"

#include "nsISchemaLoader.h"
#include "nsISchema.h"

class nsIDOMElement;
class nsIDOMNode;
class nsIXFormsXPathEvaluator;
class nsIDOMXPathResult;
class nsXFormsControl;

/**
 * Implementation of the XForms \<model\> element.
 *
 * This includes all of the code for loading the model's external resources and
 * initializing the model and its controls.
 *
 * @see http://www.w3.org/TR/xforms/slice3.html#structure-model
 *
 * @todo We need to dispatch the initial (default) events to controls (XXX)
 *
 */
class nsXFormsModelElement : public nsXFormsStubElement,
                             public nsIModelElementPrivate,
                             public nsISchemaLoadListener,
                             public nsIDOMEventListener
{
public:
  nsXFormsModelElement() NS_HIDDEN;

  NS_DECL_ISUPPORTS_INHERITED
  NS_DECL_NSIXFORMSMODELELEMENT
  NS_DECL_NSIMODELELEMENTPRIVATE
  NS_DECL_NSISCHEMALOADLISTENER
  NS_DECL_NSIWEBSERVICEERRORHANDLER
  NS_DECL_NSIDOMEVENTLISTENER

  // nsIXTFGenericElement overrides
  NS_IMETHOD OnDestroyed();
  NS_IMETHOD GetScriptingInterfaces(PRUint32 *aCount, nsIID ***aArray);
  NS_IMETHOD WillChangeDocument(nsIDOMDocument *aNewDocument);
  NS_IMETHOD DocumentChanged(nsIDOMDocument *aNewDocument);
  NS_IMETHOD DoneAddingChildren();
  NS_IMETHOD HandleDefault(nsIDOMEvent *aEvent, PRBool *aHandled);
  NS_IMETHOD OnCreated(nsIXTFGenericElementWrapper *aWrapper);

  // Called after nsXFormsAtoms is registered
  static NS_HIDDEN_(void) Startup();

private:

  NS_HIDDEN_(already_AddRefed<nsIDOMDocument>)
    FindInstanceDocument(const nsAString &aID);

  NS_HIDDEN_(nsresult) FinishConstruction();
  NS_HIDDEN_(void)     MaybeNotifyCompletion();

  NS_HIDDEN_(nsresult)   ProcessBind(nsIXFormsXPathEvaluator *aEvaluator,
                                   nsIDOMNode                *aContextNode,
                                   PRInt32                   aContextPosition,
                                   PRInt32                   aContextSize,
                                   nsIDOMElement             *aBindElement);

  NS_HIDDEN_(void)     RemoveModelFromDocument();

  /**
   * Dispatch the necessary events to a control, aControl, when the instance
   * node, aNode, it is bound to changes state.
   */
  NS_HIDDEN_(void)     DispatchEvents(nsIXFormsControl *aControl,
                                      nsIDOMNode       *aNode);

  // Returns true when all external documents have been loaded
  PRBool IsComplete() const { return (mSchemaTotal == mSchemaCount
                                      && mPendingInstanceCount == 0);  }

  nsIDOMElement            *mElement;
  nsCOMPtr<nsISchemaLoader> mSchemas;
  nsStringArray             mPendingInlineSchemas;
  nsVoidArray               mFormControls;

  PRInt32 mSchemaCount;
  PRInt32 mSchemaTotal;
  PRInt32 mPendingInstanceCount;

  /** The MDG for this model */
  nsXFormsMDGEngine mMDG;

  /** List of changed nodes, ie. nodes that have not been informed about changes yet */
  nsXFormsMDGSet    mChangedNodes;

  // This flag indicates whether or not the document fired DOMContentLoaded
  PRBool mDocumentLoaded;
};

NS_HIDDEN_(nsresult) NS_NewXFormsModelElement(nsIXTFElement **aResult);

#endif
