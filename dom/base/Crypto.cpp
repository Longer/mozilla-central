/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "Crypto.h"
#include "nsIDOMClassInfo.h"
#include "DOMError.h"
#include "nsString.h"
#include "nsIRandomGenerator.h"
#include "jsapi.h"
#include "jsfriendapi.h"

using namespace js::ArrayBufferView;

namespace mozilla {
namespace dom {

NS_INTERFACE_MAP_BEGIN(Crypto)
  NS_INTERFACE_MAP_ENTRY(nsISupports)
  NS_INTERFACE_MAP_ENTRY(nsIDOMCrypto)
  NS_DOM_INTERFACE_MAP_ENTRY_CLASSINFO(Crypto)
NS_INTERFACE_MAP_END

NS_IMPL_ADDREF(Crypto)
NS_IMPL_RELEASE(Crypto)

Crypto::Crypto()
{
  MOZ_COUNT_CTOR(Crypto);
}

Crypto::~Crypto()
{
  MOZ_COUNT_DTOR(Crypto);
}

NS_IMETHODIMP
Crypto::GetRandomValues(const jsval& aData, JSContext *cx, jsval* _retval)
{
  // Make sure this is a JavaScript object
  if (!aData.isObject()) {
    return NS_ERROR_DOM_NOT_OBJECT_ERR;
  }

  JSObject* view = &aData.toObject();

  // Make sure this object is an ArrayBufferView
  if (!JS_IsTypedArrayObject(view)) {
    return NS_ERROR_DOM_TYPE_MISMATCH_ERR;
  }

  // Throw if the wrong type of ArrayBufferView is passed in
  // (Part of the Web Crypto API spec)
  switch (JS_GetArrayBufferViewType(view)) {
    case TYPE_INT8:
    case TYPE_UINT8:
    case TYPE_UINT8_CLAMPED:
    case TYPE_INT16:
    case TYPE_UINT16:
    case TYPE_INT32:
    case TYPE_UINT32:
      break;
    default:
      return NS_ERROR_DOM_TYPE_MISMATCH_ERR;
  }

  uint32_t dataLen = JS_GetTypedArrayByteLength(view);

  if (dataLen == 0) {
    NS_WARNING("ArrayBufferView length is 0, cannot continue");
    return NS_OK;
  } else if (dataLen > 65536) {
    return NS_ERROR_DOM_QUOTA_EXCEEDED_ERR;
  }

  nsCOMPtr<nsIRandomGenerator> randomGenerator;
  nsresult rv;
  randomGenerator =
    do_GetService("@mozilla.org/security/random-generator;1", &rv);
  if (NS_FAILED(rv)) {
    NS_WARNING("unable to continue without random number generator");
    return rv;
  }

  void *dataptr = JS_GetArrayBufferViewData(view);
  NS_ENSURE_TRUE(dataptr, NS_ERROR_FAILURE);

  unsigned char* data =
    static_cast<unsigned char*>(dataptr);

  uint8_t *buf;
  rv = randomGenerator->GenerateRandomBytes(dataLen, &buf);
  NS_ENSURE_SUCCESS(rv, NS_ERROR_FAILURE);

  memcpy(data, buf, dataLen);
  NS_Free(buf);

  *_retval = OBJECT_TO_JSVAL(view);

  return NS_OK;
}

#ifndef MOZ_DISABLE_CRYPTOLEGACY
// Stub out the legacy nsIDOMCrypto methods. The actual
// implementations are in security/manager/ssl/src/nsCrypto.{cpp,h}

NS_IMETHODIMP
Crypto::GetVersion(nsAString & aVersion)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
Crypto::GetEnableSmartCardEvents(bool *aEnableSmartCardEvents)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
Crypto::SetEnableSmartCardEvents(bool aEnableSmartCardEvents)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
Crypto::GenerateCRMFRequest(nsIDOMCRMFObject * *_retval)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
Crypto::ImportUserCertificates(const nsAString & nickname,
                               const nsAString & cmmfResponse,
                               bool doForcedBackup, nsAString & _retval)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
Crypto::PopChallengeResponse(const nsAString & challenge,
                             nsAString & _retval)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
Crypto::Random(int32_t numBytes, nsAString & _retval)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
Crypto::SignText(const nsAString & stringToSign, const nsAString & caOption,
                 nsAString & _retval)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
Crypto::Logout()
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
Crypto::DisableRightClick()
{
  return NS_ERROR_NOT_IMPLEMENTED;
}
#endif

} // namespace dom
} // namespace mozilla
