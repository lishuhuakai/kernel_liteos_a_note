/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dynlink.h"

#define CFI_SUCCESS             0
#define CFI_FAILED              -1
/* Define LIBRARY_ALIGNMENT to balance the size of CFI shadow and the number of DSOs that each process can load.
 * The LIBRARY_ALIGNMENT is use to calculate the start address of a DSO. Each DSO should be in the different
 * LIBRARY_ALIGNMENT memory range so that each DSO can be well mapped to the CFI shadow.
 */
#define LIBRARY_ALIGNMENT_BITS  18
#define LIBRARY_ALIGNMENT       (1UL << LIBRARY_ALIGNMENT_BITS)

/* map all the dso and the dependents to cfi shadow */
int init_cfi_shadow(struct dso *dso_list, struct dso *ldso);
/* map a dso and the dependents to cfi shadow */
int map_dso_to_cfi_shadow(struct dso *dso);
/* unmap a dso from cfi shadow */
void unmap_dso_from_cfi_shadow(struct dso *dso);