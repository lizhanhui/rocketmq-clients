/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "TlsHelper.h"

#include "MixAll.h"
#include "OpenSSLCompatible.h"
#include <memory>
#include <openssl/hmac.h>

ROCKETMQ_NAMESPACE_BEGIN

std::string TlsHelper::sign(const std::string& access_secret, const std::string& content) {
  HMAC_CTX* ctx = HMAC_CTX_new();
  HMAC_Init_ex(ctx, access_secret.c_str(), access_secret.length(), EVP_sha1(), nullptr);
  HMAC_Update(ctx, reinterpret_cast<const unsigned char*>(content.c_str()), content.length());
  auto result = new unsigned char[EVP_MD_size(EVP_sha1())];
  unsigned int len;
  HMAC_Final(ctx, result, &len);
  HMAC_CTX_free(ctx);

  std::string hex_str = MixAll::hex(result, len);
  delete[] result;
  return hex_str;
}

const char* TlsHelper::CA = R"(
-----BEGIN CERTIFICATE-----
MIIGCDCCA/CgAwIBAgICEAAwDQYJKoZIhvcNAQELBQAwgZwxCzAJBgNVBAYTAkNO
MRIwEAYDVQQIDAlaaGUgSmlhbmcxEjAQBgNVBAcMCUhhbmcgWmhvdTEPMA0GA1UE
CgwGQXBhY2hlMREwDwYDVQQLDAhSb2NrZXRNUTEcMBoGA1UEAwwTcm9ja2V0bXEu
YXBhY2hlLm9yZzEjMCEGCSqGSIb3DQEJARYUbGl6aGFuaHVpQGFwYWNoZS5vcmcw
HhcNMjEwNDI1MDkwMjE5WhcNMzEwNDIzMDkwMjE5WjCBiDELMAkGA1UEBhMCQ04x
EjAQBgNVBAgMCVpoZSBKaWFuZzEPMA0GA1UECgwGQXBhY2hlMREwDwYDVQQLDAhS
b2NrZXRNUTEcMBoGA1UEAwwTcm9ja2V0bXEuYXBhY2hlLm9yZzEjMCEGCSqGSIb3
DQEJARYUbGl6aGFuaHVpQGFwYWNoZS5vcmcwggIiMA0GCSqGSIb3DQEBAQUAA4IC
DwAwggIKAoICAQDLL1rQ3A6YnWwdAT3rcf7wXK2DN6AwkWfGexp5o8y+dPj49D5b
PhEVIDEjidtWASFfCunJq9Ku48K/U8tha58JPL2CJ3773Mw7CU3LwD46Ft32+kGM
3+rXuXi44Fs714U/jJkxA6AsJdl4VtgjfXGWhbTq7ZT//l3tECH7Q096cp4xXATx
0RuvlaE33k8SpOPB+oHyjdFTwSW9HuZx71dLNCoZ0b52teJDVJFMACz6R5t3ZglF
MUosUzEUKdCXqBDvjOzMI2GHr7NYKAWHnOPNcYbf8jKzQ5msW+1MOfacVX+YUmKr
4Tf8NZHTSA4MuSNZ46gVFK7tYau2FDAkBWSb4hnuqqoGn58QtwrRBWtCmnLc4133
6sgEGoJJR5M7WF0usl7KAgqgwy6xdU1joHO4SzvpysQ9kHWr1hcTqpeLD/svxLpZ
3x0WrhIAQoyd4uIHv/501LgD2KE5YnBa4S10LYrZeQpoJO5YV56HJdyvqNgVh2fn
tIRJAKq/1Wvw6izK/zOC6ZC+ug3n9vPqCgpBI0NFm3aR4AcSx70FsmlfrjUAsluL
kIzQwuu3Ip7hUS9kBVUxmvaD2yy2HPc+UPiYm4Y2YxqTx2j11APbN32M8rj1mroE
d9JFUV3WjbY+1QEAQ9upWTJLnOzS4a8yNsXIdMoKeU68Lyi/F3wA9ciK9wIDAQAB
o2YwZDAdBgNVHQ4EFgQUebBfwZjpYrRBUX9XL89qA0LzaXEwHwYDVR0jBBgwFoAU
3NYFR3h8OuGYCHVl1gk2yFoM8LIwEgYDVR0TAQH/BAgwBgEB/wIBADAOBgNVHQ8B
Af8EBAMCAYYwDQYJKoZIhvcNAQELBQADggIBAJdwY+mAgtdAkVB8gRwcql7YdmWM
vlc3OYr14DHQT8eN5ItGou8MvTUR3wxCz72788/GHG1dLl54cnxzWjLgZZdapfQL
hsl1Iz6JSzyMTrNZ0HiuRw9J5XnRUIhWAO9fpRV4f/ce9J4o9YMFt//GW9WJrL59
860gNL+L1IaWDWdnPlTYnQc3ndhnStelURANtsDp808t5RO8w94/Tp/6+KgqXwSj
uAxeHNVoMcNcr/Cff/fkLXQjSlSnpX7+Yf4y9pPSq7z6QHdsfSVq9FL51cXUdK5W
XRzkWK2QKp82CRz2jbrXd9Q7rjrWtDq8vgpKsZg+myLd55wPjiscM/vZmf4LjjFC
ubfKrclMh9CFtSsk38p21l3MBl4hyULsqVHAvz4jMrTaNB+1vYQ4OhgiqxB1KJff
hw2sHZfthHLm8W+mOma/LwA+u4QFE3ihq6bjgAZxte56HUuQwa7y4FifFcZi05ih
CnXNRo8Q8napmPTwtG9XKk9HQwLSMH+wTMnrV9R8Nt4ZkPlsJV8uY/zKOfOGHsY/
+nR5vQ046Y8MT9z26dZe5MUsGUf9706+yAQAQAwzZiYJ+HiWijAIVynITMAZuEh9
JfwS66kGVNOlwqMkbAEO61tuGFmy5iRBnXOoUM2mbaGVvnbS5iBCkH6M69oq5vip
42zKYBLmpEvL4PdS
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIGIDCCBAigAwIBAgIJAN/jxbRuUfSjMA0GCSqGSIb3DQEBCwUAMIGcMQswCQYD
VQQGEwJDTjESMBAGA1UECAwJWmhlIEppYW5nMRIwEAYDVQQHDAlIYW5nIFpob3Ux
DzANBgNVBAoMBkFwYWNoZTERMA8GA1UECwwIUm9ja2V0TVExHDAaBgNVBAMME3Jv
Y2tldG1xLmFwYWNoZS5vcmcxIzAhBgkqhkiG9w0BCQEWFGxpemhhbmh1aUBhcGFj
aGUub3JnMB4XDTIxMDQyNTA4NTMyMVoXDTQxMDQyMDA4NTMyMVowgZwxCzAJBgNV
BAYTAkNOMRIwEAYDVQQIDAlaaGUgSmlhbmcxEjAQBgNVBAcMCUhhbmcgWmhvdTEP
MA0GA1UECgwGQXBhY2hlMREwDwYDVQQLDAhSb2NrZXRNUTEcMBoGA1UEAwwTcm9j
a2V0bXEuYXBhY2hlLm9yZzEjMCEGCSqGSIb3DQEJARYUbGl6aGFuaHVpQGFwYWNo
ZS5vcmcwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIKAoICAQDWKlLdC6uqiPA9
egWqBxAmKCB47ALsh7gO0nxa/0Mkr/QsRfvUtRHMoTqxLJ/IiFXmy5ZHtXq6VppL
CWE3cjN6nbYhxAyP5zmXYLQVf3BAwRqGl/ZVfZLEUvgLACMTpLTNbsqtF90UVF1K
txdsfpfKgyYp5Sff0rll94enuOfoAk18oI6ukxw5rMR3vqk7XYWdvqbEfPKr3Hiw
6r7B/JxFLv+FJIdmmHb4dgQ26Jcs9B6e2MKLXm1LRtNBI9ko6eE6UZmYkK22D/YZ
T65o8j+ZxMQPHEI/evNZp+5PfV+/061d+dtC+qgqmUTdYREmotfYtdFDNNYhZuou
GuvIG4yb02I8bYkz9ZjcWrSlLkjsODQGlF3iAy7k8HX7e952D7W/ZtXcU/1iQNVn
o0Q27QAyuTlWlBcsmSsWBiJWo6FXUDz4ODtPEXnNLNHVcRTq4EIxmMada+0nSSOu
S39XKVGiPpC/4nzxtL3kD/DAw1loTEyJxz5QNOKm34IacCbmDlJ0eqi9vPMy7Fl7
VZ5hc/yfFh1FEUlkuc4oQJftbZbUnd6osZjX9AJVX3yW+NgYtezInMhma2sGy4wn
ie2x0C8TxXF5dRrgzCk/cka3rp/InFkWx7OlJ3QLkYI6YIzbSQuF9xzJK5zFoMGP
rBCylmS1f+Xf2EbXfpek/ZZLBnzmHwIDAQABo2MwYTAdBgNVHQ4EFgQU3NYFR3h8
OuGYCHVl1gk2yFoM8LIwHwYDVR0jBBgwFoAU3NYFR3h8OuGYCHVl1gk2yFoM8LIw
DwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAYYwDQYJKoZIhvcNAQELBQAD
ggIBAAUCkPkzYeXNluXxrJnQowDsi9z8klpD1XkgB3e8j4eOMiqby1TfPIQVRZS9
d18iENpMFszF8fwEp0mMYhH4amHvL12qjdiepeYV57HmYUWeJIxvp/6xaJjcYJLE
jJILlNdDvZdKDwJZ2iCvV95KCZ0sp3kFgI1ZHnbLPuGw79lNAwVIfp2+/2QEq6Fu
TYEeWAbTaIQaUpQr2DpJShKkri4crdTN780dsfeTV0behD9MFmJPtpxL678KbF8a
j/A6lFWuts7/uwNXV+SR4wJMJGcxU1cRCsB99NGc0KIEZR92OiOg7U9cDdw5s448
yhm6+9zk2FKobXkBp25IaCyiwmHo8JP0Ht8sCKIORKGnXWKmwhQmZDIty+d+Jwi+
EuLnYEmgepgm4PYfehLvPo6krJrsyqfytJRcEyvlnWWobYv2S7jvpEcii/7huFqc
tBaxBXqLiIYxCWnygImeViquYBTu8jv87QfA8zCLSYVWWMitNnZE/Cw6RiCdlXdf
SIxY0S364T4z3zoO4i2Tl3dtgMb8qf6plvVe2mgbZxkXvkeSzGUJlvpovV2gW8jD
+agCIhmMjuYuBTsfWyj5cAfibcZSKb0xzehRUTtzX+Uc7GCiX8P5u2tgDBnxQUzM
dpafYF4v5HJmOyUNoNWdX6DhysaUK/sq4so8fW9vjgTQ5o2r
-----END CERTIFICATE-----)";

const char* TlsHelper::client_certificate_chain = R"(
-----BEGIN CERTIFICATE-----
MIIGDjCCA/agAwIBAgICEAIwDQYJKoZIhvcNAQELBQAwgYgxCzAJBgNVBAYTAkNO
MRIwEAYDVQQIDAlaaGUgSmlhbmcxDzANBgNVBAoMBkFwYWNoZTERMA8GA1UECwwI
Um9ja2V0TVExHDAaBgNVBAMME3JvY2tldG1xLmFwYWNoZS5vcmcxIzAhBgkqhkiG
9w0BCQEWFGxpemhhbmh1aUBhcGFjaGUub3JnMB4XDTIxMDQyNTEyMjIxOVoXDTMx
MDQyMzEyMjIxOVowgZ4xCzAJBgNVBAYTAkNOMRIwEAYDVQQIDAlaaGUgSmlhbmcx
EjAQBgNVBAcMCUhhbmcgWmhvdTEPMA0GA1UECgwGQWxpeXVuMQswCQYDVQQLDAJN
UTEdMBsGA1UEAwwUY2xpZW50Lm1xLmFsaXl1bi5jb20xKjAoBgkqhkiG9w0BCQEW
G3NodXRpYW4ubHpoQGFsaWJhYmEtaW5jLmNvbTCCASIwDQYJKoZIhvcNAQEBBQAD
ggEPADCCAQoCggEBALNtRLFYGNpCzIPKC3wHMxxtPOWFl5Wl5wj9TZLYntyQ+h5C
x6OaZzdqCIconAqM+F+glShoe9y4MQn4tS4cg5UpwfJjAD2XJiri56S8L2tB5t45
5mTUF3iMoncq07NQZdWwVJcrkdxawwNLmBJjdfnALrCLrK8gejtsa/eClLArR3Ep
WlgSchxTvCWduYmb8bJFfHP5dZ1aT4rXJkUA+lDvm73N2B06f5HDazvNSxXqQ8AY
DnqKXPkUkA53gOgNXRwmY6TFfQLp3qzxD13ueUuUC/zonn4EifTdcp6xsYLjv+Fd
Fd0feGTHfeGi1Pp9nvogYbGzPeC7CaSrLphuCl0CAwEAAaOCAWgwggFkMAkGA1Ud
EwQCMAAwEQYJYIZIAYb4QgEBBAQDAgZAMDMGCWCGSAGG+EIBDQQmFiRPcGVuU1NM
IEdlbmVyYXRlZCBTZXJ2ZXIgQ2VydGlmaWNhdGUwHQYDVR0OBBYEFO40rO1cUjC4
CvSeLx+79vnzlXVFMIHKBgNVHSMEgcIwgb+AFHmwX8GY6WK0QVF/Vy/PagNC82lx
oYGipIGfMIGcMQswCQYDVQQGEwJDTjESMBAGA1UECAwJWmhlIEppYW5nMRIwEAYD
VQQHDAlIYW5nIFpob3UxDzANBgNVBAoMBkFwYWNoZTERMA8GA1UECwwIUm9ja2V0
TVExHDAaBgNVBAMME3JvY2tldG1xLmFwYWNoZS5vcmcxIzAhBgkqhkiG9w0BCQEW
FGxpemhhbmh1aUBhcGFjaGUub3JnggIQADAOBgNVHQ8BAf8EBAMCBaAwEwYDVR0l
BAwwCgYIKwYBBQUHAwEwDQYJKoZIhvcNAQELBQADggIBAJtAPRUp3pLU+u860tZR
315Kxv2UHf/Mz+hqAa5N7xuojVq61VkcK+Q5SkV88B1Male3aadQcRjddGdDPTMX
c/mw6mZ43xuu2uPB47PRkCEbmwIpAiaY9nAKo0fQeHaM1roYtf20A864C0u76YsK
Gl+j8Gry0kYFMLEoVLjbdr4XtqkJOZCaHErsVjf83YSFPiyQaq6GpuHpGEyMoj+h
MM7Pcb5aqpehs9cRgYoU2IS2VXQZ6W71z5IHzw9xJngI8oZ94hwWzWyRBWl/u3jb
eNfVdh5baC9JjjJBUFedj5J8ayPl7Fh9e7YQDPhe2og42D9fWr+SmgM06QnU6fAG
s+n1ocKjpT+Rm+0Q9+BkKKebISrgP92cyiVpb2m7kCH2m52pnvEQ4GGXt5gVAHgA
UTRu0hDrkJtouu8lh5wpiu1oskgUgsYTKEpQkVB00Ich1pVxNVROWmLRHAJHVskl
MYcNAYihk4RVmyGwIEX37SWyMRKIjHY3loKJIwRJoMPduGXPk6E5NmhTRYSwKigo
pHjfDLXeAlTSYIEHUZMpTHNjigkNq5wSYYWs9BYoFaMg4k+n5m88cCsbkb9m+0b0
BBtZdWGVKbQK0YkyS+p9UULVhYg82JaF/IHLm/yR0LcmrCUp7qK6iLlXxSSD3OVP
79fXjr+aaE5Mskph9yNKFAKi
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIGCDCCA/CgAwIBAgICEAAwDQYJKoZIhvcNAQELBQAwgZwxCzAJBgNVBAYTAkNO
MRIwEAYDVQQIDAlaaGUgSmlhbmcxEjAQBgNVBAcMCUhhbmcgWmhvdTEPMA0GA1UE
CgwGQXBhY2hlMREwDwYDVQQLDAhSb2NrZXRNUTEcMBoGA1UEAwwTcm9ja2V0bXEu
YXBhY2hlLm9yZzEjMCEGCSqGSIb3DQEJARYUbGl6aGFuaHVpQGFwYWNoZS5vcmcw
HhcNMjEwNDI1MDkwMjE5WhcNMzEwNDIzMDkwMjE5WjCBiDELMAkGA1UEBhMCQ04x
EjAQBgNVBAgMCVpoZSBKaWFuZzEPMA0GA1UECgwGQXBhY2hlMREwDwYDVQQLDAhS
b2NrZXRNUTEcMBoGA1UEAwwTcm9ja2V0bXEuYXBhY2hlLm9yZzEjMCEGCSqGSIb3
DQEJARYUbGl6aGFuaHVpQGFwYWNoZS5vcmcwggIiMA0GCSqGSIb3DQEBAQUAA4IC
DwAwggIKAoICAQDLL1rQ3A6YnWwdAT3rcf7wXK2DN6AwkWfGexp5o8y+dPj49D5b
PhEVIDEjidtWASFfCunJq9Ku48K/U8tha58JPL2CJ3773Mw7CU3LwD46Ft32+kGM
3+rXuXi44Fs714U/jJkxA6AsJdl4VtgjfXGWhbTq7ZT//l3tECH7Q096cp4xXATx
0RuvlaE33k8SpOPB+oHyjdFTwSW9HuZx71dLNCoZ0b52teJDVJFMACz6R5t3ZglF
MUosUzEUKdCXqBDvjOzMI2GHr7NYKAWHnOPNcYbf8jKzQ5msW+1MOfacVX+YUmKr
4Tf8NZHTSA4MuSNZ46gVFK7tYau2FDAkBWSb4hnuqqoGn58QtwrRBWtCmnLc4133
6sgEGoJJR5M7WF0usl7KAgqgwy6xdU1joHO4SzvpysQ9kHWr1hcTqpeLD/svxLpZ
3x0WrhIAQoyd4uIHv/501LgD2KE5YnBa4S10LYrZeQpoJO5YV56HJdyvqNgVh2fn
tIRJAKq/1Wvw6izK/zOC6ZC+ug3n9vPqCgpBI0NFm3aR4AcSx70FsmlfrjUAsluL
kIzQwuu3Ip7hUS9kBVUxmvaD2yy2HPc+UPiYm4Y2YxqTx2j11APbN32M8rj1mroE
d9JFUV3WjbY+1QEAQ9upWTJLnOzS4a8yNsXIdMoKeU68Lyi/F3wA9ciK9wIDAQAB
o2YwZDAdBgNVHQ4EFgQUebBfwZjpYrRBUX9XL89qA0LzaXEwHwYDVR0jBBgwFoAU
3NYFR3h8OuGYCHVl1gk2yFoM8LIwEgYDVR0TAQH/BAgwBgEB/wIBADAOBgNVHQ8B
Af8EBAMCAYYwDQYJKoZIhvcNAQELBQADggIBAJdwY+mAgtdAkVB8gRwcql7YdmWM
vlc3OYr14DHQT8eN5ItGou8MvTUR3wxCz72788/GHG1dLl54cnxzWjLgZZdapfQL
hsl1Iz6JSzyMTrNZ0HiuRw9J5XnRUIhWAO9fpRV4f/ce9J4o9YMFt//GW9WJrL59
860gNL+L1IaWDWdnPlTYnQc3ndhnStelURANtsDp808t5RO8w94/Tp/6+KgqXwSj
uAxeHNVoMcNcr/Cff/fkLXQjSlSnpX7+Yf4y9pPSq7z6QHdsfSVq9FL51cXUdK5W
XRzkWK2QKp82CRz2jbrXd9Q7rjrWtDq8vgpKsZg+myLd55wPjiscM/vZmf4LjjFC
ubfKrclMh9CFtSsk38p21l3MBl4hyULsqVHAvz4jMrTaNB+1vYQ4OhgiqxB1KJff
hw2sHZfthHLm8W+mOma/LwA+u4QFE3ihq6bjgAZxte56HUuQwa7y4FifFcZi05ih
CnXNRo8Q8napmPTwtG9XKk9HQwLSMH+wTMnrV9R8Nt4ZkPlsJV8uY/zKOfOGHsY/
+nR5vQ046Y8MT9z26dZe5MUsGUf9706+yAQAQAwzZiYJ+HiWijAIVynITMAZuEh9
JfwS66kGVNOlwqMkbAEO61tuGFmy5iRBnXOoUM2mbaGVvnbS5iBCkH6M69oq5vip
42zKYBLmpEvL4PdS
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIGIDCCBAigAwIBAgIJAN/jxbRuUfSjMA0GCSqGSIb3DQEBCwUAMIGcMQswCQYD
VQQGEwJDTjESMBAGA1UECAwJWmhlIEppYW5nMRIwEAYDVQQHDAlIYW5nIFpob3Ux
DzANBgNVBAoMBkFwYWNoZTERMA8GA1UECwwIUm9ja2V0TVExHDAaBgNVBAMME3Jv
Y2tldG1xLmFwYWNoZS5vcmcxIzAhBgkqhkiG9w0BCQEWFGxpemhhbmh1aUBhcGFj
aGUub3JnMB4XDTIxMDQyNTA4NTMyMVoXDTQxMDQyMDA4NTMyMVowgZwxCzAJBgNV
BAYTAkNOMRIwEAYDVQQIDAlaaGUgSmlhbmcxEjAQBgNVBAcMCUhhbmcgWmhvdTEP
MA0GA1UECgwGQXBhY2hlMREwDwYDVQQLDAhSb2NrZXRNUTEcMBoGA1UEAwwTcm9j
a2V0bXEuYXBhY2hlLm9yZzEjMCEGCSqGSIb3DQEJARYUbGl6aGFuaHVpQGFwYWNo
ZS5vcmcwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIKAoICAQDWKlLdC6uqiPA9
egWqBxAmKCB47ALsh7gO0nxa/0Mkr/QsRfvUtRHMoTqxLJ/IiFXmy5ZHtXq6VppL
CWE3cjN6nbYhxAyP5zmXYLQVf3BAwRqGl/ZVfZLEUvgLACMTpLTNbsqtF90UVF1K
txdsfpfKgyYp5Sff0rll94enuOfoAk18oI6ukxw5rMR3vqk7XYWdvqbEfPKr3Hiw
6r7B/JxFLv+FJIdmmHb4dgQ26Jcs9B6e2MKLXm1LRtNBI9ko6eE6UZmYkK22D/YZ
T65o8j+ZxMQPHEI/evNZp+5PfV+/061d+dtC+qgqmUTdYREmotfYtdFDNNYhZuou
GuvIG4yb02I8bYkz9ZjcWrSlLkjsODQGlF3iAy7k8HX7e952D7W/ZtXcU/1iQNVn
o0Q27QAyuTlWlBcsmSsWBiJWo6FXUDz4ODtPEXnNLNHVcRTq4EIxmMada+0nSSOu
S39XKVGiPpC/4nzxtL3kD/DAw1loTEyJxz5QNOKm34IacCbmDlJ0eqi9vPMy7Fl7
VZ5hc/yfFh1FEUlkuc4oQJftbZbUnd6osZjX9AJVX3yW+NgYtezInMhma2sGy4wn
ie2x0C8TxXF5dRrgzCk/cka3rp/InFkWx7OlJ3QLkYI6YIzbSQuF9xzJK5zFoMGP
rBCylmS1f+Xf2EbXfpek/ZZLBnzmHwIDAQABo2MwYTAdBgNVHQ4EFgQU3NYFR3h8
OuGYCHVl1gk2yFoM8LIwHwYDVR0jBBgwFoAU3NYFR3h8OuGYCHVl1gk2yFoM8LIw
DwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAYYwDQYJKoZIhvcNAQELBQAD
ggIBAAUCkPkzYeXNluXxrJnQowDsi9z8klpD1XkgB3e8j4eOMiqby1TfPIQVRZS9
d18iENpMFszF8fwEp0mMYhH4amHvL12qjdiepeYV57HmYUWeJIxvp/6xaJjcYJLE
jJILlNdDvZdKDwJZ2iCvV95KCZ0sp3kFgI1ZHnbLPuGw79lNAwVIfp2+/2QEq6Fu
TYEeWAbTaIQaUpQr2DpJShKkri4crdTN780dsfeTV0behD9MFmJPtpxL678KbF8a
j/A6lFWuts7/uwNXV+SR4wJMJGcxU1cRCsB99NGc0KIEZR92OiOg7U9cDdw5s448
yhm6+9zk2FKobXkBp25IaCyiwmHo8JP0Ht8sCKIORKGnXWKmwhQmZDIty+d+Jwi+
EuLnYEmgepgm4PYfehLvPo6krJrsyqfytJRcEyvlnWWobYv2S7jvpEcii/7huFqc
tBaxBXqLiIYxCWnygImeViquYBTu8jv87QfA8zCLSYVWWMitNnZE/Cw6RiCdlXdf
SIxY0S364T4z3zoO4i2Tl3dtgMb8qf6plvVe2mgbZxkXvkeSzGUJlvpovV2gW8jD
+agCIhmMjuYuBTsfWyj5cAfibcZSKb0xzehRUTtzX+Uc7GCiX8P5u2tgDBnxQUzM
dpafYF4v5HJmOyUNoNWdX6DhysaUK/sq4so8fW9vjgTQ5o2r
-----END CERTIFICATE-----
)";

const char* TlsHelper::client_private_key = R"(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEAs21EsVgY2kLMg8oLfAczHG085YWXlaXnCP1Nktie3JD6HkLH
o5pnN2oIhyicCoz4X6CVKGh73LgxCfi1LhyDlSnB8mMAPZcmKuLnpLwva0Hm3jnm
ZNQXeIyidyrTs1Bl1bBUlyuR3FrDA0uYEmN1+cAusIusryB6O2xr94KUsCtHcSla
WBJyHFO8JZ25iZvxskV8c/l1nVpPitcmRQD6UO+bvc3YHTp/kcNrO81LFepDwBgO
eopc+RSQDneA6A1dHCZjpMV9AunerPEPXe55S5QL/OiefgSJ9N1ynrGxguO/4V0V
3R94ZMd94aLU+n2e+iBhsbM94LsJpKsumG4KXQIDAQABAoIBAQCQigN+QZOzr4AE
4mwh+8LGsZEO7HFSUhUtfrBRDUPcNsXmPrGIL8faMgawqpwZO7qRflDp/lFZWt/W
Qo3hmYK+VKcyVWJKO0q53ZPxds9UPgB3IHli3CAUMmZmTQ/BMZD3dVaPeL/FnS/f
pbPZjFnrWU/DVKFFaopM+Gh5OK4Cw2Be704vGdqoLLSAixEpAPh+U7UFyDO2Er6r
ZewpqKz6fgwzn/FIXIe+OSlIg+5IyDNBYJkXDtAAd4tCxAflnLbbcIoHjeASiAZy
qg9ax5s2+rm1yRM2Eds/jMX6vKqCTWeOq7T8Sz5FpgS9F6hL70LnBy6dxwK2MYKr
lGjNc4wZAoGBAOX51pit7YuMk1vPO5wV8TUzX7jVz9N1zuuIybpDuhZ0y7gdM8L4
LP+gaQpsdoeFnKCYC7oGjqLLrfXVxARqq1FLIaMTpd4C3uHAyWQmoZjBpFiw5a9f
FhZ67uDMEYTWoegAkDKV4O/AnfLkoDSJexh4eZ7LnChoEtGth2C4uuTPAoGBAMe7
E1hXctDszEDQCbfzkvz4zSJZ32CzauYFCVxLz+4xQVRDKQccMwVnbDSjA0nq1RmN
c7COrEGQ4EkTgy/KGfo+KeEGoDWq3UG/EO+6lBSTaWD1M0E1nqidasPx3CuhZ81d
dAaqo4Hgm+QZOFPx7bCdNAZw8u/RS+ELxanEwcETAoGBAK/DiqeWBn8pKVZ5w+0/
N+KZKQdzIitTVpndKZAaQxb2nNxSNUVFt96HEsDMtKpPC64RhqTebhUt5f6FKhLW
E6lF6qdgVNyhBQc0+LZ6b/b3yEugp2qpGZCE1B0q2hjUkVlnVM3Ujt2uMQl6gpak
nFfPe+RJaUfYgsmN/nGFvhExAoGAA/4LTvLAisiQ9KKz7wdhYskeYJdnTamIc3ya
SUwCpgI6ChMlmMzJgEwm745qnGmjRNV9AIR5kP3yXcOUFbp2dngTojndrfTyY0QM
dJ5II3km3BdKpP0LZNdwsOX+vqiFSKebkB6NEiWJODAA3jyxwZwC1DYhxQVr/N2d
X91c+h0CgYEA4PnAEmTILyJ9OPwV2A1cQ/RPi4smxZ0IIXZt/pmiEV4MYR5ULT7g
XzK3CHvV0wghQdBIxfAs8H31Ayux6N0wnva0w3tHesw15ByiDeNeJKO+tjpc01Zq
3xU6hUkWYGclvlezeWPQFkJKWYCLX4uw8lWOLCGd8VY2OZXJkS6W608=
-----END RSA PRIVATE KEY-----)";

ROCKETMQ_NAMESPACE_END
