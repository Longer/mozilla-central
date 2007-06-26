/*
 * ConditionMet.java
 *
 * Created on June 26, 2007, 2:12 PM
 *
 */

/*
 * $Id: Condition.java,v 1.1 2007-06-26 12:39:04 edburns%acm.org Exp $
 */

/*
 * The contents of this file are subject to the terms
 * of the Common Development and Distribution License
 * (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at
 * https://javaserverfaces.dev.java.net/CDDL.html or
 * legal/CDDLv1.0.txt.
 * See the License for the specific language governing
 * permission and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL
 * Header Notice in each file and include the License file
 * at legal/CDDLv1.0.txt.
 * If applicable, add the following below the CDDL Header,
 * with the fields enclosed by brackets [] replaced by
 * your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * [Name of File] [ver.__] [Date]
 *
 * Copyright 2005 Sun Microsystems Inc. All Rights Reserved
 */

package org.mozilla.mcp;

/**
 *
 * @author edburns
 */
public class Condition {
    
    protected boolean conditionMet = false;
    public boolean isConditionMet() {
        return conditionMet;
    }
    
    public void setConditionMet(boolean newValue) {
        conditionMet = newValue;
    }
    
}
