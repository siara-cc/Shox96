/*
 * Copyright (C) 2019 Siara Logics (cc)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author Arundale R.
 *
 */
#ifndef shox96_0_1_0
#define shox96_0_1_0
long shox96_0_1_0_compress(char *in, long len, char *out, C_CONTEXT *c_ctx);
int shox96_0_1_0_decompress(char *in, long len, char *out, D_CONTEXT *d_ctx);
#endif

